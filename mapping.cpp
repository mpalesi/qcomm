#include <iostream>
#include <cassert>
#include "mapping.h"

void Mapping::display()
{
  cout << endl
       << "*** Mapping ***" << endl;
  
  int nqb = qubit2core.size();    
  for (int qb=0; qb<nqb; qb++)
    {
      assert(isMapped(qb));
      cout << "qubit " << qb << " -> core " << qubit2core[qb] << endl;
    }
}

map<int,int> Mapping::defaultMapping(int nqubits, int ncores)
{
  map<int,int> q2c;
  
  int core_id = 0;
  for (int qb=0; qb<nqubits; qb++)
    {
      q2c[qb] = core_id;
      
      core_id = (core_id + 1) % ncores;
    }
  
  return q2c;
}

bool Mapping::isMapped(const int qb) const
{
  return (qubit2core.find(qb) != qubit2core.end());
}

int Mapping::qubit2CoreSafe(const int qb) const
{
  assert(qubit2core.find(qb) != qubit2core.end());

  return qubit2core.at(qb);
}
