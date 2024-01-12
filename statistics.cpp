#include <iostream>
#include "statistics.h"

using namespace std;

void Statistics::display() const
{
  cout << endl
       << "*** Statistics ***" << endl
       << "executed_gates: " << executed_gates << endl;
  
  communication_time.display();
  cout << "computation_time: " << computation_time << endl
       << "execution_time: " << computation_time + communication_time.getTotalTime()
       << endl;
}
