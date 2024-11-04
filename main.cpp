#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <limits>
#include <random>
#include <algorithm>
#include "utils.h"
#include "architecture.h"
#include "gate.h"
#include "core.h"
#include "circuit.h"
#include "communication.h"
#include "mapping.h"
#include "statistics.h"
#include "communication_time.h"
#include "noc.h"
#include "parameters.h"

using namespace std;


			   
// ----------------------------------------------------------------------
bool isLocalGate(const Gate& gate, const Mapping& mapping)
{
  assert(!gate.empty());
  
  int core_id = mapping.qubit2core[gate.front()]; // core where the first qubit of the gate is mapped to

  for (const auto& qb : gate)
    if (mapping.qubit2core[qb] != core_id)
      return false;

  return true;
}

// ----------------------------------------------------------------------
void splitLocalRemoteGates(const ParallelGates& pgates, const Mapping& mapping,
			   ParallelGates& lgates, ParallelGates& rgates)
{
  lgates.clear();
  rgates.clear();
  
  for (const auto& gate : pgates)
    {
      if (isLocalGate(gate, mapping))
	lgates.push_back(gate);
      else
	rgates.push_back(gate);
    }
}

// ----------------------------------------------------------------------
Statistics LocalExecution(const ParallelGates& lgates,
			  const Parameters& params)
{
  Statistics stats;

  if (!lgates.empty())
    {
      stats.computation_time = params.gate_delay;
      // TODO: add swap contribution like in the remote execution
      stats.executed_gates = lgates.size();
    }
  
  return stats;
}

// ----------------------------------------------------------------------
// Choose the target core based on the number of allocated
// qubits. Select the core with the fewest allocated qubits.
int selectDestinationCore(const Gate& gate, const Mapping& mapping, const Cores& cores)
{
  int min_qb = numeric_limits<int>::max();
  int selected_core = -1;
  for (const auto& qb : gate)
    {
      int core_id = mapping.qubit2core[qb];

      if ((int)cores.cores[core_id].size() < min_qb)
	{
	  min_qb = cores.cores[core_id].size();
	  selected_core = core_id;
	}
    }

  assert(selected_core != -1);
  
  return selected_core;
}

// ----------------------------------------------------------------------
// qubits in gate are allocated to dst_core. Both mapping and cores
// structures are updated accordingly.
void updateMappingAndCores(const Architecture& architecture,
			   Mapping& mapping, Cores& cores,
			   const Gate& gate, const int dst_core)
{
  for (const auto& qb : gate)
    {
      int src_core = mapping.qubit2core[qb];

      if (src_core != dst_core)
	{
	  mapping.qubit2core[qb] = dst_core;
	  cores.cores[dst_core].insert(qb);
	  assert((int)cores.cores[dst_core].size() < architecture.qubits_per_core);
	  
	  auto it = cores.cores[src_core].find(qb);
	  assert(it != cores.cores[src_core].end());
	  cores.cores[src_core].erase(it);
	}
    }
}

// ----------------------------------------------------------------------
// Generate communications from the core where the qubits of gate are
// mapped onto to the dst_core and insert them into
// parallel_communications
void addParallelCommunications(ParallelCommunications& parallel_communications,
			       const Gate& gate, const int dst_core,
			       const Mapping& mapping)
{
  for (const auto& qb : gate)
    {
      int src_core = mapping.qubit2core[qb];
      if (src_core != dst_core)
	{
	  Communication comm(src_core, dst_core);
	  parallel_communications.push_back(comm); // insert(comm);
	}
    }
}

// ----------------------------------------------------------------------
void getCoreXY(const int core_id, const Architecture& arch,
	       int& x, int& y)
{
  x = core_id % arch.mesh_x;
  y = core_id / arch.mesh_x;
}

// ----------------------------------------------------------------------
double getClassicalCommunicationTime(const Communication& comm,
				     const Architecture& arch,
				     const Parameters& params)
{
  int src_x, src_y, dst_x, dst_y;
  
  getCoreXY(comm.src_core, arch, src_x, src_y);
  getCoreXY(comm.dst_core, arch, dst_x, dst_y);

  int hops = abs(src_x - dst_x) + abs(src_y - dst_y);

  return hops * params.hop_delay;
}

// ----------------------------------------------------------------------
CommunicationTime getCommunicationTime(const ParallelCommunications& pcomms,
				       const NoC& noc,
				       const Parameters& params)
{
  CommunicationTime ct;

  ct.t_epr = params.epr_delay;
  ct.t_dist = params.dist_delay;
  ct.t_pre = params.pre_delay;
  ct.t_clas = noc.getCommunicationTime(pcomms);
  ct.t_post = params.post_delay;
  
  return ct;
}

// ----------------------------------------------------------------------
void addCommunicationTime(CommunicationTime& total_ct,
			  const CommunicationTime& ct)
{
  total_ct.t_epr  += ct.t_epr;
  total_ct.t_dist += ct.t_dist;
  total_ct.t_pre  += ct.t_pre;
  total_ct.t_clas += ct.t_clas;
  total_ct.t_post += ct.t_post;
}

// ----------------------------------------------------------------------
void updateRemoteExecutionStats(Statistics& stats,
				const ParallelGates& pgates,
				const ParallelCommunications& pcomms,
				const NoC& noc,
				const Parameters& params)
{
  stats.executed_gates += pgates.size();

  stats.intercore_comms += pcomms.size();
    
  CommunicationTime comm_time = getCommunicationTime(pcomms, noc, params);
  				       
  addCommunicationTime(stats.communication_time, comm_time);

  // We assume that all the gates in the slice are executed
  // concurrently, each contributing with a gate delay, assuming that
  // the gate delay is constant regardless of the gate type.
  stats.computation_time += params.gate_delay;
}

// ----------------------------------------------------------------------
void removeExecutedGates(const ParallelGates& scheduled_gates,
			 ParallelGates& gates)
{
  // Remove scheduled_gates from gates
  gates.remove_if([&scheduled_gates](const Gate &gate) {
    return std::find(scheduled_gates.begin(), scheduled_gates.end(), gate) != scheduled_gates.end();
    });
}

// ----------------------------------------------------------------------
Statistics RemoteExecution(const Architecture& architecture, const NoC& noc,
			   const Parameters& parameters,
			   const ParallelGates& rgates,
			   Mapping& mapping, Cores& cores)
{
  Statistics stats;

  if (!rgates.empty())
    {
      ParallelGates gates = rgates;

      while (!gates.empty())
	{
	  vector<int> available_ltm_ports(architecture.number_of_cores, architecture.ltm_ports);
	  ParallelGates parallel_gates;
	  ParallelCommunications parallel_communications;
	  
	  bool first_gate_to_map = true;
	  for (const auto& gate : gates)
	    {
	      bool skip_this_gate = false;
	      int dst_core = selectDestinationCore(gate, mapping, cores);
	      vector<int> tmp_available_ltm_ports = available_ltm_ports;
	      for (const auto& qb : gate)
		{		  
		  int src_core = mapping.qubit2core[qb];
		  if (src_core != dst_core)
		    {
		      if (tmp_available_ltm_ports[src_core] && tmp_available_ltm_ports[dst_core])
			{
			  // qb can be teleported from src_core to dst_core
			  tmp_available_ltm_ports[src_core]--;
			  tmp_available_ltm_ports[dst_core]--;
			}
		      else
			{
			  if (first_gate_to_map)
			    assert(false); // at least the first gate of the parallel_gates must be mapped!
			  skip_this_gate = true;
			  break;
			}
		    }
		} // for (const auto& qb : gate)

	      if (!skip_this_gate)
		{
		  // IMPORTANT: addParallelCommunication must be called before updateMappingAndCores
		  addParallelCommunications(parallel_communications, gate, dst_core, mapping); 
		  available_ltm_ports = tmp_available_ltm_ports;
		  parallel_gates.push_back(gate);
		  updateMappingAndCores(architecture, mapping, cores, gate, dst_core);
		}

	      first_gate_to_map = false;

	    } // for (const auto& gate : gates)
	  
	  updateRemoteExecutionStats(stats, parallel_gates, parallel_communications,
				     noc, parameters);
	  cores.saveHistory();
	  removeExecutedGates(parallel_gates, gates);
	} //  while (!gates.empty())
    }
  
  return stats;
}

// ----------------------------------------------------------------------
Statistics MergeLocalRemoteStatistics(const Statistics& stats_local,
				      const Statistics& stats_remote)
{
  Statistics stats;

  // TODO: check this function!!!
  stats.executed_gates = stats_local.executed_gates + stats_remote.executed_gates;
  stats.intercore_comms = stats_remote.intercore_comms;
  
  stats.communication_time = stats_remote.communication_time;
  stats.computation_time = (stats_local.computation_time > stats_remote.computation_time) ?
    stats_local.computation_time : stats_remote.computation_time;
  
  return stats;
}

// ----------------------------------------------------------------------
// Simulate the execution of parallel gates
Statistics Simulate(const ParallelGates& pgates, const Architecture& architecture,
		    const NoC& noc, const Parameters& parameters,
		    Mapping& mapping, Cores& cores)
{
  ParallelGates lgates, rgates;

  splitLocalRemoteGates(pgates, mapping, lgates, rgates);  

  assert(!rgates.empty() || !lgates.empty());

  Statistics stats_local = LocalExecution(lgates, parameters);
  Statistics stats_remote = RemoteExecution(architecture, noc, parameters,
					    rgates, mapping, cores);

  return MergeLocalRemoteStatistics(stats_local, stats_remote);
}

// ----------------------------------------------------------------------
// Simulate the entire circuit
Statistics Simulate(const Circuit& circuit, const Architecture& architecture,
		    const NoC& noc, const Parameters& parameters,
		    Mapping& mapping, Cores& cores)
{
  Statistics global_stats;
  
  for (const auto& parallel_gates : circuit.circuit)
    {
      Statistics stats = Simulate(parallel_gates, architecture, noc,
				  parameters, mapping, cores);

      /*	
      double th = noc.getThroughput(stats.intercore_comms,
				    stats.computation_time + stats.communication_time.getTotalTime());
      */
      
      double th = noc.getThroughput(stats.intercore_comms,
				    stats.communication_time.getTotalTime());

      global_stats.updateStatistics(stats, th);
    }

  return global_stats;
}

// ----------------------------------------------------------------------
bool checkCommandLine(int argc, char* argv[],
		      string& circuitfn, string& architecturefn, string& parametersfn,
		      map<string,string>& params_override)
{
  if (argc < 7)
    return false;

  params_override.clear();
  
  for (int i=1; i<argc; i++)
    {
      string arg = argv[i];
      if (arg == "-a")
	architecturefn = string(argv[++i]);
      else if (arg == "-c")
	circuitfn = string(argv[++i]);
      else if (arg == "-p")
	parametersfn = string(argv[++i]);
      else if (arg == "-o")
	{
	  params_override[string(argv[i+1])] = string(argv[i+2]);
	  i += 2;
	}
      else
	return false;
    }  

  return true;
}

// ----------------------------------------------------------------------
void overrideParameters(const map<string,string>& params_override,
			Architecture& arch, Parameters& params)
{
  for (auto it = params_override.begin(); it != params_override.end(); ++it)
    {
      string param = it->first;
      string value = it->second;

      if (param == "mesh_x")
	arch.updateMeshX(stoi(value));
      else if (param == "mesh_y")	
	arch.updateMeshY(stoi(value));
      else if (param == "link_width")	
	arch.updateLinkWidth(stoi(value));
      else if (param == "qubits_per_core")
	arch.updateQubitsPerCore(stoi(value));
      else if (param == "ltm_ports")
	arch.updateLTMPorts(stoi(value));
      else if (param == "radio_channels")
	arch.updateRadioChannels(stoi(value));
      else if (param == "wireless_enabled")
	arch.updateWirelessEnabled(stoi(value));
      else if (param == "hop_delay")
	params.updateHopDelay(stod(value));
      else if (param == "epr_delay")
	params.updateEPRDelay(stod(value));
      else if (param == "pre_delay")
	params.updatePreDelay(stod(value));
      else if (param == "post_delay")
	params.updatePostDelay(stod(value));
      else if (param == "wbit_rate")
	params.updateWBitRate(stod(value));
      else if (param == "token_pass_time")
	params.updateTokenPassTime(stod(value));
      else
	cout << ">>> Unrecognized parameter '" << param << "' is ignored!" << endl;
    }
}

// ----------------------------------------------------------------------
int main(int argc, char* argv[])
{
  string circuit_fn, architecture_fn, parameters_fn;
  map<string,string> params_override; // parameter name -> value
  
  if (!checkCommandLine(argc, argv, circuit_fn, architecture_fn, parameters_fn, params_override))
    {
      cout << "Usage " << argv[0] << " -c <circuit> -a <architecture> -p <parameters> [-o <param> <value>]" << endl;
      
      assert(false);
    }

  
  Circuit circuit;
  if (!circuit.readFromFile(circuit_fn))
    {
      cout << "error reading circuit file" << endl;
      assert(false);
    }
  
  Architecture architecture;
  if (!architecture.readFromFile(architecture_fn))
    {
      cout << "error reading architecture file" << endl;
      assert(false);
    }
  
  Parameters parameters;
  if (!parameters.readFromFile(parameters_fn))
    {
      cout << "error reading parameters file" << endl;
      assert(false);
    }

  overrideParameters(params_override, architecture, parameters);

  circuit.display(false);
  architecture.display();
  parameters.display();

  NoC noc(architecture.mesh_x, architecture.mesh_y, architecture.link_width, parameters.hop_delay, architecture.qubits_per_core);
  if (architecture.wireless_enabled)
    noc.enableWiNoC(parameters.wbit_rate, architecture.radio_channels, parameters.token_pass_time);
      
  noc.display();
  
  Mapping mapping(circuit.number_of_qubits, architecture.number_of_cores);

  Cores cores(architecture, mapping);
  cores.display();

  Statistics stats = Simulate(circuit, architecture, noc, parameters, mapping, cores);
  stats.display(cores, architecture);
  
  
  return 0;
}
