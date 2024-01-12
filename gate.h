#ifndef __GATE_H__
#define __GATE_H__

#include <list>

using namespace std;

typedef list<int> Gate;
typedef list<Gate> ParallelGates;

void displayGate(const Gate& gate, bool newline);
void displayGates(const ParallelGates& gates, bool newline);


#endif
