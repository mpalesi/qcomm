#ifndef __MAPPING_H__
#define __MAPPING_H__

#include <vector>

using namespace std;

struct Mapping
{
  vector<int> qubit2core; // Indicates where a qubit is mapped onto which core

  Mapping() {}

  Mapping(int nqubits, int ncores) {
    qubit2core = this->defaultMapping(nqubits, ncores);
  }

  void display();

  
  vector<int> defaultMapping(int nqubits, int ncores);
};

#endif
