// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: main.cpp
// Description: Implementation of the main simulator
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <map>
#include <cassert>
#include "utils.h"
#include "architecture.h"
#include "core.h"
#include "circuit.h"
#include "mapping.h"
#include "statistics.h"
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
      cerr << "Usage " << argv[0] << " -c <circuit> -a <architecture> -p <parameters> [-o <param> <value>]" << endl;
      
      return -1;
    }

  
  Circuit circuit;
  if (!circuit.readFromFile(circuit_fn))
    {
      cerr << "Error reading circuit file " << circuit_fn << endl;
      return ERR_CIRC_FILE;
    }

  NoC noc;
  Mapping mapping;
  Cores cores(mapping);
  Architecture architecture(cores, noc);
  if (!architecture.readFromFile(architecture_fn))
    {
      cerr << "Error reading architecture file " << architecture_fn << endl;
      return ERR_ARCH_FILE;
    }
  
  Parameters parameters(noc);
  if (!parameters.readFromFile(parameters_fn))
    {
      cerr << "Error reading parameters file " << parameters_fn << endl;
      return ERR_PARM_FILE;
    }

  overrideParameters(params_override, architecture, parameters);

  // IMPORTANT: The following initializations go in this exact order -
  // Do not change order
  noc.initializeTokenOwnerMap();
  cores.mapping.initMapping(circuit.number_of_qubits,
			    architecture.number_of_cores, architecture.mapping_type);
  cores.initCores(architecture.number_of_cores, architecture.qubits_per_core);

  // Display info: banner, commandline, circuit, architecture,
  // parameters
  showBanner();

  showCommandLine(argc, argv);
  
  circuit.display(false);
  architecture.display();
  parameters.display();

  // Run simulation
  Simulation simulation;
  Statistics stats = simulation.simulate(circuit, architecture, noc, parameters, mapping, cores);

  // Display statistics
  simulation.display();
  
  stats.display(cores, architecture, parameters);
  
  
  return 0;
}
