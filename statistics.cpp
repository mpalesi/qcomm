// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: statistics.cpp
// Description: Implementation of statistics structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <limits>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "statistics.h"

using namespace std;


Statistics::Statistics()
{
  executed_gates = 0;
  total_intercore_comms = 0;
  intercore_volume = 0;
  computation_time = 0.0;
  avg_throughput = 0.0;
  max_throughput = 0.0;
  samples = 0;
  fetch_time = 0.0;
  decode_time = 0.0;
  dispatch_time = 0.0;
}

Statistics::Statistics(const int ncores) : Statistics()
{
  intercore_comms = vector<vector<int>>(ncores, vector<int>(ncores, 0));
}


double Statistics::getExecutionTime() const
{
  return (computation_time + communication_time.getTotalTime() + fetch_time + decode_time + dispatch_time);
}


// return core idx where qb is located. -1 if not found
int Statistics::qbitToCore(const int qb, const vector<Core> cores)
{
  int n = cores.size();

  for (int i=0; i<n; i++)
    if (cores[i].find(qb) != cores[i].end())
      return i;

  return -1;
}

void Statistics::displayIntercoreCommunications()
{
  for (int s=0; s<(int)intercore_comms.size(); s++)
    {
      cout << IND << IND << "- [";
      for (int d=0; d<(int)intercore_comms[s].size(); d++)
	{
	  cout << intercore_comms[s][d];
	  if (d != (int)intercore_comms[s].size()-1)
	    cout << ", ";
	  else
	    cout << "]";
	}
      cout << endl;
    }
}

void Statistics::displayOperationsPerQubit()
{
  cout << "{";

  auto it = operations_per_qubit.begin();
  auto last = prev(operations_per_qubit.end());
  for (; it != operations_per_qubit.end(); ++it) {
    cout << "q" << it->first << ": " << it->second;
    if (it != last)
      cout << ", ";
    }  
  cout << "}" << endl;
}

void Statistics::displayTeleportationsPerQubit()
{
  cout << "{";

  auto it = teleportations_per_qubit.begin();
  auto last = prev(teleportations_per_qubit.end());
  for (; it != teleportations_per_qubit.end(); ++it) {
    cout << "q" << it->first << ": " << it->second;
    if (it != last)
      cout << ", ";
    }  
  cout << "}" << endl;
}

void Statistics::display(const Cores& cores, const Architecture& arch,
			 const Parameters& params)
{
  cout << endl
       << "Statistics:" << endl
       << IND << "executed_gates: " << executed_gates << endl
       << IND << "total_intercore_communications: " << total_intercore_comms << endl
       << IND << "intercore_traffic_volume: " << intercore_volume << " # bits" << endl
       << IND << "throughput:" << endl
       << IND << IND << "avg: " << avg_throughput/1.0e6 << " # Mbps" << endl
       << IND << IND << "peak: " << max_throughput/1.0e6 << " # Mbps" << endl;

  double avg, min, max;
  getCoresStats(cores.history, arch, avg, min, max);
  cout << IND << "core_utilization:" << endl
       << IND << IND << "avg: " << avg << endl
       << IND << IND << "tmin: " << min << endl
       << IND << IND << "tmax: " << max << endl;


  if (params.stats_detailed)
    {
      cout << IND << "intercore_communications: # row is source, col is target" << endl;
      displayIntercoreCommunications();
      
      cout << IND << "operations_per_qubit: ";
      displayOperationsPerQubit();

      cout << IND << "teleportations_per_qubit: ";
      displayTeleportationsPerQubit();
    }
  
  communication_time.display(IND);
  
  double execution_time = getExecutionTime();
  cout << IND << "computation_time: " << computation_time << " # sec" << endl
       << IND << "fetch_time: " << fetch_time << " # sec" << endl
       << IND << "decode_time: " << decode_time << " # sec" << endl
       << IND << "dispatch_time: " << dispatch_time << " # sec" << endl
       << IND << "execution_time: " << execution_time << " # sec" << endl
       << IND << "coherence: " << 100.0 * computeCoherence(execution_time, params.t1) << " # %" << endl;
}

void Statistics::updateStatistics(const Statistics& stats)
{
  executed_gates += stats.executed_gates;
  total_intercore_comms += stats.total_intercore_comms;
  intercore_volume += stats.intercore_volume;
  computation_time += stats.computation_time;
  
  communication_time.t_epr += stats.communication_time.t_epr;
  communication_time.t_dist += stats.communication_time.t_dist;
  communication_time.t_pre += stats.communication_time.t_pre;
  communication_time.t_clas += stats.communication_time.t_clas;
  communication_time.t_post += stats.communication_time.t_post;

  fetch_time += stats.fetch_time;
  decode_time += stats.decode_time;
  dispatch_time += stats.dispatch_time;

  double th = stats.intercore_volume / stats.getExecutionTime();
    
  // update throughput stats
  if (th > 0.0)
    {      
      if (th > max_throughput)
	max_throughput = th;

      avg_throughput = (samples * avg_throughput) / (1 + samples) +
	th / (samples + 1);

      samples++;
  }

  // Accumulate intercore_comms
  int ncores = intercore_comms.size();
  for (int i = 0; i < ncores; ++i)
    for (int j = 0; j < ncores; ++j)
      intercore_comms[i][j] += stats.intercore_comms[i][j];

  // Accumulate teleportations_per_qubit
  for (const auto& pair : stats.teleportations_per_qubit)
    teleportations_per_qubit[pair.first] += pair.second;

  // Accumulated operations_per_qubit
  for (const auto& pair : stats.operations_per_qubit)
    operations_per_qubit[pair.first] += pair.second;
}


void Statistics::getCoresStats(const vector<Core>& cores, const Architecture& arch,
			       double& avg_u, double& min_u, double& max_u)
{
  min_u = numeric_limits<double>::max();
  max_u = numeric_limits<double>::min();
  double sum_u = 0.0;
  
  for (const auto& core : cores)
    {
      double utilization = (double)core.size() / arch.qubits_per_core;
      sum_u += utilization;
      if (utilization < min_u) min_u = utilization;
      if (utilization > max_u) max_u = utilization;      
    }

  avg_u = sum_u / cores.size();
}

void Statistics::getCoresStats(const list<vector<Core> >& history, const Architecture& arch,
			       double& avg_u, double& min_u, double& max_u)
{
  double _avg_u, _min_u, _max_u;
  double sum_avg_u = 0.0;
  min_u = numeric_limits<double>::max();
  max_u = numeric_limits<double>::min();
  
  for (const auto& coresh : history)
    {
      getCoresStats(coresh, arch, _avg_u, _min_u, _max_u);

      sum_avg_u += _avg_u;
      if (_min_u < min_u) min_u = _min_u;
      if (_max_u > max_u) max_u = _max_u;
    }

  avg_u = sum_avg_u / history.size();
}

void Statistics::addIntercoreCommunications(const ParallelCommunications& pcomms)
{
  for (const auto& comm : pcomms)
    intercore_comms[comm.src_core][comm.dst_core]++;
}

void Statistics::addTeleportationsPerQubit(const int qb)
{
  teleportations_per_qubit[qb]++;
}

void Statistics::addOperationsPerQubit(const ParallelGates& pgates, const int overhead)
{
  for (const auto& gate : pgates)
    for (const auto& qb : gate.second)
      operations_per_qubit[qb] += 1 + overhead;
}
