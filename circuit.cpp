// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: circuit.cpp
// Description: Implementation of circuit structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cassert>
#include <map>
#include "utils.h"
#include "circuit.h"

using namespace std;

void Circuit::display(const bool verbose)
{
  cout << endl
       << "Circuit:" << endl
       << IND << "number_of_qubits: " << number_of_qubits << endl
       << IND << "number_of_gates: " << number_of_gates << endl
       << IND << "number_of_stages: " << number_of_stages << endl;

  // Compute gate distribution
  map<int,int> inputhist;
  for (const auto& pg : circuit)
    for (const auto& g : pg)
      inputhist[g.second.size()]++;
  cout << IND << "distribution_of_gates:" << endl;
  for (const auto& hp : inputhist)
    cout << IND << IND << "'" << hp.first << "-input': " << hp.second*100.0/number_of_gates << " # %" << endl;
  
  if (verbose)
    for (const auto& parallel_gates : circuit)
      displayGates(parallel_gates, true);
}

bool Circuit::readFromFile(const std::string& file_name)
{
  circuit.clear();
  ifstream input_file(file_name);
  if (!input_file.is_open())
    return false;

  int min_qubit = std::numeric_limits<int>::max();
  int max_qubit = std::numeric_limits<int>::min();
  string line;

  while (getline(input_file, line))
  {
    istringstream iss(line);
    ParallelGates parallel_gates;

    string token;
    while (iss >> token)
    {
      // If token doesn't end in ')', keep reading until full gate string is assembled
      while (!token.empty() && token.back() != ')' && iss)
      {
        string next;
        iss >> next;
        token += " " + next;
      }

      // Now token should be in form GATENAME(q1 q2 ...)
      size_t open_par = token.find('(');
      size_t close_par = token.find(')');

      if (open_par == std::string::npos || close_par == std::string::npos || close_par <= open_par)
      {
        std::cerr << "Invalid gate format: " << token << std::endl;
        return false;
      }

      Gate gate;
      gate.first = token.substr(0, open_par); // gate name
      std::string args = token.substr(open_par + 1, close_par - open_par - 1); // qubit list inside ()

      std::istringstream args_stream(args);
      int qubit;
      while (args_stream >> qubit)
      {
        gate.second.push_back(qubit);
        if (qubit < min_qubit) min_qubit = qubit;
        if (qubit > max_qubit) max_qubit = qubit;
      }

      parallel_gates.push_back(gate);
      number_of_gates++;
    }

    if (!parallel_gates.empty())
    {
      circuit.push_back(parallel_gates);
      number_of_stages++;
    }
  }

  input_file.close();

  if (min_qubit != 0)
  {
    std::cerr << "Qubit indices must start from 0" << std::endl;
    return false;
  }

  number_of_qubits = max_qubit - min_qubit + 1;

  return true;
}


void Circuit::generateCircuit(const int nqubits, const int ngates,
			      const vector<float>& gateprob)
{
  circuit.clear();
  int gatecount = 0;
  int nstages = 0;
  set<int> used_qubits;
  ParallelGates parallel_gates;

  while (gatecount != ngates)
    {
      int fanin = getRandomNumber(gateprob) + 1;
      set<int> qubits = getRandomNoRepetition(nqubits, fanin);

      set<int> intersection_set;	
      set_intersection(used_qubits.begin(), used_qubits.end(),
		       qubits.begin(), qubits.end(),
		       inserter(intersection_set, intersection_set.begin()));

      if (intersection_set.empty())
	{
	  // add the gate into the parallel_gates set
	  Gate gate;
	  // the name of the gate is Gn where n is the number of inputs
	  gate.first = "G" + to_string(qubits.size());
	  gate.second = list<int>(qubits.begin(), qubits.end());	  
	  parallel_gates.push_back(gate);
	  gatecount++;
	  used_qubits.insert(qubits.begin(), qubits.end());
	}

      if (!intersection_set.empty() || gatecount == ngates)
	{
	  assert(!parallel_gates.empty());
	  circuit.push_back(parallel_gates);

	  // start a new stage
	  parallel_gates.clear();
	  used_qubits.clear();
	  nstages++;
	}
    }

  // update attributes
  number_of_qubits = nqubits;
  number_of_gates = ngates;
  number_of_stages = nstages;
}
