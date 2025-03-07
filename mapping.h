#ifndef __MAPPING_H__
#define __MAPPING_H__

#include <map>

using namespace std;

struct Mapping
{
  map<int,int> qubit2core; // Indicates where a qubit is mapped onto which core

  Mapping() {}

  Mapping(int nqubits, int ncores) {
    qubit2core = this->defaultMapping(nqubits, ncores);
  }

  void display();

  map<int,int> defaultMapping(int nqubits, int ncores);

  bool isMapped(const int qb) const;
  int qubit2CoreSafe(const int qb) const;
};

#endif
