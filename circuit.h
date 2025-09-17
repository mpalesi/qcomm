// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: circuit.h
// Description: Implementation of circuit structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include <list>
#include <vector>
#include "gate.h"

struct Circuit
{
  list<ParallelGates> circuit;
  int number_of_qubits;
  int number_of_gates;
  int number_of_stages;

  Circuit(): number_of_qubits(0), number_of_gates(0), number_of_stages(0) {}

  void display(const bool verbose = true);

  bool readFromFile(const string& file_name);

  void generateCircuit(const int nqubits, const int ngates,
		       const vector<float>& gateprob);

  // Circuit is fixed in such a way qubit 0 is always present
  void fixCircuit();
};

#endif
