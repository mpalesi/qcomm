// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: communication_tipe.cpp
// Description: Implementation of communication_time structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <iomanip>
#include "communication_time.h"

using namespace std;

void CommunicationTime::display(const string& prefix) const
{
  double total_time = getTotalTime();
  
  cout << prefix << "communication_time: " << total_time << " # sec " << endl;

  cout << prefix << "communication_time_components:" << endl
       << prefix << prefix << "epr_pair_generation_time:" << endl
       << prefix << prefix << prefix << "time: " << t_epr << " # sec" << endl
       << prefix << prefix << prefix << "percent: " << 100*t_epr/total_time << " # %" << endl
       << prefix << prefix << "epr_pair_distribution_time:" << endl
       << prefix << prefix << prefix << "time: " << t_dist << " # sec" << endl
       << prefix << prefix << prefix << "percent: " << 100*t_dist/total_time << " # %" << endl
       << prefix << prefix << "pre_processing_time: " << endl
       << prefix << prefix << prefix << "time: " << t_pre << " # sec" << endl
       << prefix << prefix << prefix << "percent: " << 100*t_pre/total_time << " # %" << endl
       << prefix << prefix << "classical_transfer_time:" << endl
       << prefix << prefix << prefix << "time: " << t_clas << " # sec" << endl
       << prefix << prefix << prefix << "percent: " << 100*t_clas/total_time << " # %" << endl
       << prefix << prefix << "post_processing_time:" << endl
       << prefix << prefix << prefix << "time: " << t_post << " # sec" << endl
       << prefix << prefix << prefix << "percent: " << 100*t_post/total_time << " # %" << endl;
}

double CommunicationTime::getTotalTime() const
{
  return t_epr + t_dist + t_pre + t_clas + t_post;
}
