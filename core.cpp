// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: core.cpp
// Description: Implementation of core structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <cassert>
#include "utils.h"
#include "core.h"


void Cores::initCores(const int number_of_cores, const int qubits_per_core)
{
  ancilla_counter = 0;
  
  cores.resize(number_of_cores);
  
  int nqubits = mapping.qubit2core.size();
  for (int qb=0; qb<nqubits; qb++)
    {
      if (!mapping.isMapped(qb))
	{
	  cerr << "Error: qubit " << qb << " is not mapped!" << endl;
	  assert(false);
	}
      int core_no = mapping.qubit2core.at(qb);
      
      cores[core_no].insert(qb);
      
      if ((int)cores[core_no].size() > qubits_per_core)
	{
	  cerr << "Error: Number of qubits mapped on core " << core_no
	       << " exceeds its capacity." << endl;
	  assert(false);
	}
    }
}

int Cores::getNumCores() const
{
  return cores.size();
}

void Cores::saveHistory()
{
  history.push_back(cores);
}

void Cores::display()
{
  cout << IND << "Cores:" << endl;

  cout << IND << IND << "number_of_cores: " << cores.size() << endl;
  
  cout << IND << IND << "qubits_per_core:" << endl;
  int ncores = cores.size();  
  for (int core_id=0; core_id<ncores; core_id++)
    {
      cout << IND << IND << IND << "'core " << core_id << "': ";
      cout << "[";

      set<int> qbs = cores[core_id];
      auto it = qbs.begin();
      auto last = prev(qbs.end());
      for (; it != qbs.end(); ++it) {      
	cout << *it;
	if (it != last)
	  cout << ", ";
      }
      cout << "]" << endl;
    }
}

int Cores::generateAncillaId()
{
  ancilla_counter--;
  return ancilla_counter;
}

bool Cores::allocateAncilla(const int core_id,
			    const int qubits_per_core,
			    int& ancilla)
{
  Core core = cores[core_id];

  if ((int)core.size() >= qubits_per_core)
    return false;

  ancilla = generateAncillaId();
  cores[core_id].insert(ancilla);
  mapping.qubit2core[ancilla] = core_id;
  
  return true;
}

