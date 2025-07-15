#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include <vector>
#include "core.h"
#include "circuit.h"
#include "architecture.h"
#include "parameters.h"
#include "communication_time.h"
#include "communication.h"

struct Statistics
{
  int executed_gates;
  int total_intercore_comms;
  int intercore_volume;
  CommunicationTime communication_time;
  double computation_time;
  double avg_throughput, max_throughput;
  int    samples;
  double fetch_time;
  double decode_time;
  double dispatch_time;
  vector<vector<int>> intercore_comms; // row is source, col is target

  Statistics();
  Statistics(const int ncores);
  
  void updateStatistics(const Statistics& stats);
  
  void display(const Circuit& circuit, const Cores& cores, const Architecture& arch,
	       const Parameters& params);

  double getExecutionTime() const;
  
  void getCoresStats(const vector<Core>& cores, const Architecture& arch,
		     double& avg_u, double& min_u, double& max_u);
  void getCoresStats(const list<vector<Core> >& history, const Architecture& arch,
		     double& avg_u, double& min_u, double& max_u);

  void displayIntercoreCommunications(const Cores& cores);
  //  int countCommunications(const Cores& cores, const int src, const int dst);
  //  vector<vector<int> > getIntercoreCommunications(const Cores& cores);

  void displayOperationsPerQubit(const Circuit& circuit);
  vector<int> getOperationsPerQubit(const Circuit& circuit);

  void displayTeleportationsPerQubit(const Circuit& circuit, const Cores& cores);
  vector<int> getTeleportationsPerQubit(const Circuit& circuit, const Cores& cores);
  int getTeleportationsPerQubit(const int qb, const Cores& cores);
  int qbitToCore(const int qb, const vector<Core> cores);

  void addIntercoreCommunications(const ParallelCommunications& pcomms);

};

#endif
