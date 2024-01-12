#ifndef __CORE_H__
#define __CORE_H__

#include <set>
#include <vector>
#include "mapping.h"
#include "architecture.h"

typedef set<int> Core; // set of qubits mapped in the core

struct Cores
{
  vector<Core> cores;

  Cores(const Architecture& architecture, const Mapping& mapping);

  void display();
};

#endif
