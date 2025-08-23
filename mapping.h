// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: mapping.h
// Description: Declaration of mapping structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __MAPPING_H__
#define __MAPPING_H__

#include <map>

#define MAP_RANDOM     0
#define MAP_SEQUENTIAL 1

using namespace std;

struct Mapping
{
  map<int,int> qubit2core; // Indicates where a qubit is mapped onto which core
  
  Mapping() {}

  void initMapping(const int nqubits, const int ncores,
		   const int mapping_type, const unsigned seed);

  void display();

  map<int,int> sequentialMapping(const int nqubits, const int ncores);

  // chrono seed if 'seed' is 0
  map<int,int> randomMapping(const int nqubits, const int ncores,
			     const unsigned seed);

  bool isMapped(const int qb) const;
  int qubit2CoreSafe(const int qb) const;
};

#endif
