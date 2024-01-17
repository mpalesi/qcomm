#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "core.h"
#include "architecture.h"
#include "communication_time.h"

struct Statistics
{
  int executed_gates;
  CommunicationTime communication_time;
  double computation_time;
  
  Statistics() : executed_gates(0), computation_time(0.0) {}

  void updateStatistics(const Statistics& stats);
  
  void display(const Cores& cores, const Architecture& arch);

  void getCoresStats(const vector<Core>& cores, const Architecture& arch,
		     double& avg_u, double& min_u, double& max_u);
  void getCoresStats(const list<vector<Core> >& history, const Architecture& arch,
		     double& avg_u, double& min_u, double& max_u);

};

#endif
