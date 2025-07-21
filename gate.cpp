// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: gate.cpp
// Description: Implementation of gate related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include "gate.h"

//----------------------------------------------------------------------
void displayGate(const Gate& gate, bool newline)
{
  cout << gate.first << "(";
  for (auto qb = gate.second.begin(); qb != gate.second.end(); ++qb)
    {
      cout << *qb;
      if (next(qb) != gate.second.end())
	cout << " ";
    }
  cout << ") ";

  if (newline)
    cout << endl;
}
		 
// ----------------------------------------------------------------------
void displayGates(const ParallelGates& gates, bool newline)
{
  for (const auto& gate : gates)
    displayGate(gate, false);

  if (newline)
    cout << endl;
}
