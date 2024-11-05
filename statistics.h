#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "core.h"
#include "architecture.h"
#include "communication_time.h"

struct Statistics
{
  int executed_gates;
  int intercore_comms;
  CommunicationTime communication_time;
  double computation_time;
  double avg_throughput, max_throughput;
  int    samples;
  
  //  Statistics() : executed_gates(0), intercore_comms(0), computation_time(0.0), samples(0) {}

  Statistics();
  
  void updateStatistics(const Statistics& stats, const double th);
  
  void display(const Cores& cores, const Architecture& arch);

  void getCoresStats(const vector<Core>& cores, const Architecture& arch,
		     double& avg_u, double& min_u, double& max_u);
  void getCoresStats(const list<vector<Core> >& history, const Architecture& arch,
		     double& avg_u, double& min_u, double& max_u);

  void displayIntercoreCommunications(const Cores& cores);
  int countCommunications(const Cores& cores, const int src, const int dst);
  vector<vector<int> > getIntercoreCommunications(const Cores& cores);

};

#endif
