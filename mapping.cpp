#include <iostream>
#include "mapping.h"

void Mapping::display()
{
  cout << endl
       << "*** Mapping ***" << endl;
  
  int nqb = qubit2core.size();    
  for (int qb=0; qb<nqb; qb++)
    cout << "qubit " << qb << " -> core " << qubit2core[qb] << endl;
}

vector<int> Mapping::defaultMapping(int nqubits, int ncores)
{
  vector<int> q2c(nqubits, -1);
  
  int core_id = 0;
  for (int qb=0; qb<nqubits; qb++)
    {
      q2c[qb] = core_id;
      
      core_id = (core_id + 1) % ncores;
    }
  
  return q2c;
}

