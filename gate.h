// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: gate.h
// Description: Declaration of gate related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __GATE_H__
#define __GATE_H__

#include <list>
#include <string>

using namespace std;

typedef pair<string, list<int>> Gate; // <name of the gate, list of qubits>
typedef list<Gate> ParallelGates;

void displayGate(const Gate& gate, bool newline);
void displayGates(const ParallelGates& gates, bool newline);


#endif
