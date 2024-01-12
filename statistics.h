#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "communication_time.h"

struct Statistics
{
  int executed_gates;
  CommunicationTime communication_time;
  double computation_time;
  
  Statistics() : executed_gates(0), computation_time(0.0) {}
  
  void display() const;
};

#endif
