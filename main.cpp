#include <iostream>
#include <map>
#include <cassert>
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
#include "simulation.h"
#include "command_line.h"

using namespace std;

		   
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

  NoC noc(architecture.mesh_x, architecture.mesh_y, architecture.link_width, parameters.noc_clock_time,
	  ceil(log2(architecture.qubits_per_core * architecture.number_of_cores)));
  if (architecture.wireless_enabled)
    noc.enableWiNoC(parameters.wbit_rate, architecture.radio_channels, parameters.token_pass_time);
      
  noc.display();
  
  Mapping mapping(circuit.number_of_qubits, architecture.number_of_cores);

  Cores cores(architecture, mapping);
  cores.display();

  // -----
  ParallelCommunications pcomms;
  pcomms.push_back(Communication(0, 3, 8));
  pcomms.push_back(Communication(1, 3, 8));
  displayParallelCommunications(pcomms);
  
  cout << "comm time: " << noc.getCommunicationTimeWired(pcomms) << endl;
  // -----
  /*
  Simulation simulation;
  Statistics stats = simulation.simulate(circuit, architecture, noc, parameters, mapping, cores);
  
  stats.display(circuit, cores, architecture);
  */  
  
  return 0;
}
