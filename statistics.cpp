#include <iostream>
#include <limits>
#include "statistics.h"

using namespace std;

Statistics::Statistics()
{
  executed_gates = 0;
  intercore_comms = 0;
  computation_time = 0.0;
  avg_throughput = 0.0;
  max_throughput = 0.0;
  samples = 0;
}

void Statistics::display(const Cores& cores, const Architecture& arch)
{
  cout << endl
       << "*** Statistics ***" << endl
       << "Executed gates: " << executed_gates << endl
       << "Intercore communications: " << intercore_comms << endl
       << "Throughput (Mbps): " << avg_throughput/1.0e6 << " avg, " << max_throughput/1.0e6 << " peak"
       << endl;

  
  double avg, min, max;
  getCoresStats(cores.history, arch, avg, min, max);
  cout << "Core utilization: " << avg << " avg, " << min << " min, " << max << " max" << endl;
    
  communication_time.display();
  double execution_time = computation_time + communication_time.getTotalTime();
  cout << "Computation time (s): " << computation_time << endl
       << "Execution time (s): " << execution_time << endl
       << "Coherence (%): " << 100.0*exp(-execution_time / 268e-6) << endl;
  
}

void Statistics::updateStatistics(const Statistics& stats, const double th)
{
  executed_gates += stats.executed_gates;
  intercore_comms += stats.intercore_comms;
  computation_time += stats.computation_time;
  
  communication_time.t_epr += stats.communication_time.t_epr;
  communication_time.t_dist += stats.communication_time.t_dist;
  communication_time.t_pre += stats.communication_time.t_pre;
  communication_time.t_clas += stats.communication_time.t_clas;
  communication_time.t_post += stats.communication_time.t_post;

  // update throughput stats
  if (th > 0.0)
    {      
      if (th > max_throughput)
	max_throughput = th;

      avg_throughput = (samples * avg_throughput) / (1 + samples) +
	th / (samples + 1);

      samples++;
  }
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
