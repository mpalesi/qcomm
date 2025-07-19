#include <iostream>
#include "gate.h"

//----------------------------------------------------------------------
void displayGate(const Gate& gate, bool newline)
{
  cout << gate.first << "(";
  for (auto qb = gate.second.begin(); qb != gate.second.end(); ++qb)
    {
      cout << *qb;
      if (next(qb) != gate.second.end())
	cout << " ";
    }
  cout << ") ";

  if (newline)
    cout << endl;
}
		 
// ----------------------------------------------------------------------
void displayGates(const ParallelGates& gates, bool newline)
{
  for (const auto& gate : gates)
    displayGate(gate, false);

  if (newline)
    cout << endl;
}
