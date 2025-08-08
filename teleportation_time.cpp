// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: teleportation_tipe.cpp
// Description: Implementation of teleportation_time structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <iomanip>
#include "teleportation_time.h"

using namespace std;

void TeleportationTime::display(const string& prefix) const
{
  double total_time = getTotalTeleportationTime();
  
  cout << prefix << "teleportation_time: " << total_time << " # sec " << endl;

  cout << prefix << "teleportation_time_components:" << endl;

  cout << prefix << prefix << "epr_pair_generation_time: " << t_epr << " # sec " 
       << "(" << 100*t_epr/total_time << "%)" << endl;
  
  cout << prefix << prefix << "epr_pair_distribution_time: " << t_dist << " # sec "
       << "(" << 100*t_dist/total_time << "%)" << endl;
  
  cout << prefix << prefix << "pre_processing_time: " << t_pre << " # sec "
       << "(" << 100*t_pre/total_time << "%)" << endl;
  
  cout << prefix << prefix << "classical_transfer_time: " << t_clas << " # sec " 
       << "(" << 100*t_clas/total_time << "%)" << endl;
  
  cout << prefix << prefix << "post_processing_time: " << t_post << " # sec "
       << "(" << 100*t_post/total_time << "%)" << endl;
  
  /*
  cout << prefix << "teleportation_time_components:" << endl
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
  */
}

double TeleportationTime::getTotalTeleportationTime() const
{
  return t_epr + t_dist + t_pre + t_clas + t_post;
}
