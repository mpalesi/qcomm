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

  void initMapping(const int nqubits, const int ncores, const int mapping_type);

  void display();

  map<int,int> sequentialMapping(const int nqubits, const int ncores);
  map<int,int> randomMapping(const int nqubits, const int ncores);

  bool isMapped(const int qb) const;
  int qubit2CoreSafe(const int qb) const;
};

#endif
