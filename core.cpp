#include <iostream>
#include <cassert>
#include "core.h"

Cores::Cores(const Architecture& architecture, const Mapping& mapping)
{
  cores.resize(architecture.number_of_cores);
  
  int nqubits = mapping.qubit2core.size();
  for (int qb=0; qb<nqubits; qb++)
    {
      int core_no = mapping.qubit2core[qb];
      
      cores[core_no].insert(qb);
      
      if ((int)cores[core_no].size() > architecture.qubits_per_core)
	{
	  cout << "Number of qubits mapped on core " << core_no
	       << " exceeds its capacity." << endl;
	  assert(false);
	}
    }
}

void Cores::saveHistory()
{
  history.push_back(cores);
}

void Cores::display()
{
  cout << endl
       << "*** Cores ***" << endl;
  int ncores = cores.size();  
  for (int core_id=0; core_id<ncores; core_id++)
    {
      cout << "core " << core_id << ": ";
      
      for (const auto& qubit : cores[core_id])
	cout << qubit << " ";
      cout << endl;
    }
}
