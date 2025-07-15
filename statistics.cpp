#include <iostream>
#include <limits>
#include <algorithm>
#include <cassert>
#include "utils.h"
#include "statistics.h"

using namespace std;


Statistics::Statistics()
{
  executed_gates = 0;
  total_intercore_comms = 0;
  intercore_volume = 0;
  computation_time = 0.0;
  avg_throughput = 0.0;
  max_throughput = 0.0;
  samples = 0;
  fetch_time = 0.0;
  decode_time = 0.0;
  dispatch_time = 0.0;
}

Statistics::Statistics(const int ncores) : Statistics()
{
  intercore_comms = vector<vector<int>>(ncores, vector<int>(ncores, 0));
}


double Statistics::getExecutionTime() const
{
  return (computation_time + communication_time.getTotalTime() + fetch_time + decode_time + dispatch_time);
}

void Statistics::displayIntercoreCommunications(const Cores& cores)
{
  int ncores = cores.getNumCores();

  for (int s=0; s<ncores; s++)
    {
      for (int d=0; d<ncores; d++)
	cout << intercore_comms[s][d] << " ";
      cout << endl;
    }
}

vector<int> Statistics::getOperationsPerQubit(const Circuit& circuit)
{
  vector<int> opsqb(circuit.number_of_qubits, 0);

  for (ParallelGates pg : circuit.circuit)
    for (Gate g : pg)
      for (int qb : g)
	opsqb[qb]++;

  return opsqb;
}

void Statistics::displayOperationsPerQubit(const Circuit& circuit)
{
  vector<int> opsqb = getOperationsPerQubit(circuit);

  for (int ops : opsqb)
    cout << ops << ", ";
  cout << endl;
}

// return core idx where qb is located. -1 if not found
int Statistics::qbitToCore(const int qb, const vector<Core> cores)
{
  int n = cores.size();

  for (int i=0; i<n; i++)
    if (cores[i].find(qb) != cores[i].end())
      return i;

  return -1;
}

int Statistics::getTeleportationsPerQubit(const int qb, const Cores& cores)
{
  int tps = 0;

  for (list<vector<Core> >::const_iterator it_curr = cores.history.begin();
       it_curr != cores.history.end(); ++it_curr)
    {
      list<vector<Core> >::const_iterator it_next = next(it_curr);
      if (it_next != cores.history.end())
	{
	  int core_s = qbitToCore(qb, *it_curr);
	  int core_d = qbitToCore(qb, *it_next);
	  assert(core_s != -1 && core_d != -1);

	  if (core_s != core_d)
	    tps++;
	}
    }
  
  return tps;
}

vector<int> Statistics::getTeleportationsPerQubit(const Circuit& circuit, const Cores& cores)
{
  vector<int> tpsqb(circuit.number_of_qubits);

  for (int qb = 0; qb < circuit.number_of_qubits; qb++)
    tpsqb[qb] = getTeleportationsPerQubit(qb, cores);
  
  return tpsqb;
}

void Statistics::displayTeleportationsPerQubit(const Circuit& circuit, const Cores& cores)
{
  vector<int> tpsqb = getTeleportationsPerQubit(circuit, cores);

  for (int tps : tpsqb)
    cout << tps << ", ";
  cout << endl;
  
}

void Statistics::display(const Circuit& circuit, const Cores& cores, const Architecture& arch,
			 const Parameters& params)
{
  cout << endl
       << "*** Statistics ***" << endl
       << "Executed gates: " << executed_gates << endl
       << "Intercore communications: " << total_intercore_comms << endl
       << "Intercore traffic volume (bits): " << intercore_volume << endl
       << "Throughput (Mbps): " << avg_throughput/1.0e6 << " avg, " << max_throughput/1.0e6 << " peak"
       << endl;

  
  double avg, min, max;
  getCoresStats(cores.history, arch, avg, min, max);
  cout << "Core utilization: " << avg << " avg, " << min << " min, " << max << " max" << endl;

  if (params.stats_detailed) {
    cout << "Intercore communications (row is source, col is target):" << endl;
    displayIntercoreCommunications(cores);
  }

  if (params.stats_detailed) {
    cout << "Operations per qubit: ";
    displayOperationsPerQubit(circuit);
  }

  if (params.stats_detailed) {
    cout << "Teleportations per qubit: ";
    displayTeleportationsPerQubit(circuit, cores);
  }

  
  communication_time.display();
  double execution_time = getExecutionTime();
  cout << "Computation time (s): " << computation_time << endl
       << "Fetch time (s): " << fetch_time << endl
       << "Decode time (s): " << decode_time << endl
       << "Dispatch time (s): " << dispatch_time << endl
       << "Execution time (s): " << execution_time << endl
       << "Coherence (%): " << 100.0 * computeCoherence(execution_time, params.t1)
       << endl; 
  
}

void Statistics::updateStatistics(const Statistics& stats)
{
  executed_gates += stats.executed_gates;
  total_intercore_comms += stats.total_intercore_comms;
  intercore_volume += stats.intercore_volume;
  computation_time += stats.computation_time;
  
  communication_time.t_epr += stats.communication_time.t_epr;
  communication_time.t_dist += stats.communication_time.t_dist;
  communication_time.t_pre += stats.communication_time.t_pre;
  communication_time.t_clas += stats.communication_time.t_clas;
  communication_time.t_post += stats.communication_time.t_post;

  cout << "fetch_time into update = " << stats.fetch_time << endl;
  fetch_time += stats.fetch_time;
  decode_time += stats.decode_time;
  dispatch_time += stats.dispatch_time;

  double th = stats.intercore_volume / stats.getExecutionTime();
    
  // update throughput stats
  if (th > 0.0)
    {      
      if (th > max_throughput)
	max_throughput = th;

      avg_throughput = (samples * avg_throughput) / (1 + samples) +
	th / (samples + 1);

      samples++;
  }

  // Accumulate intercore_comms
  int ncores = intercore_comms.size();
  for (int i = 0; i < ncores; ++i)
    for (int j = 0; j < ncores; ++j)
      intercore_comms[i][j] += stats.intercore_comms[i][j];
}


void Statistics::getCoresStats(const vector<Core>& cores, const Architecture& arch,
			       double& avg_u, double& min_u, double& max_u)
{
  min_u = numeric_limits<double>::max();
  max_u = numeric_limits<double>::min();
  double sum_u = 0.0;
  
  for (const auto& core : cores)
    {
      double utilization = (double)core.size() / arch.qubits_per_core;
      sum_u += utilization;
      if (utilization < min_u) min_u = utilization;
      if (utilization > max_u) max_u = utilization;      
    }

  avg_u = sum_u / cores.size();
}

void Statistics::getCoresStats(const list<vector<Core> >& history, const Architecture& arch,
			       double& avg_u, double& min_u, double& max_u)
{
  double _avg_u, _min_u, _max_u;
  double sum_avg_u = 0.0;
  min_u = numeric_limits<double>::max();
  max_u = numeric_limits<double>::min();
  
  for (const auto& coresh : history)
    {
      getCoresStats(coresh, arch, _avg_u, _min_u, _max_u);

      sum_avg_u += _avg_u;
      if (_min_u < min_u) min_u = _min_u;
      if (_max_u > max_u) max_u = _max_u;
    }

  avg_u = sum_avg_u / history.size();
}

void Statistics::addIntercoreCommunications(const ParallelCommunications& pcomms)
{
  for (const auto& comm : pcomms)
    intercore_comms[comm.src_core][comm.dst_core]++;
}

