// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: core.h
// Description: Declaration of core structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __CORE_H__
#define __CORE_H__

#include <set>
#include <vector>
#include <list>
#include "mapping.h"
#include "architecture.h"

typedef set<int> Core; // set of qubits mapped in the core

struct Cores
{
  vector<Core> cores;
  list<vector<Core> > history;
  int ancilla_counter;
  
  Cores(const Architecture& architecture, const Mapping& mapping);

  int getNumCores() const;
  
  bool allocateAncilla(const int core_id,
		       const Architecture& architecture,
		       Mapping& mapping, int& ancilla);
  int generateAncillaId();
  
  void saveHistory();
  
  void display(); 
};

#endif
