// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: parameters.h
// Description: Declaration of parameters structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include <string>
#include <map>
using namespace std;

struct Parameters
{
  map<string,double> gate_delays; // <gate name, delay>
  double epr_delay;
  double dist_delay;
  double pre_delay;
  double post_delay;
  double noc_clock_time;
  double wbit_rate;
  double token_pass_time;
  double memory_bandwidth; // bits/sec
  int    bits_instruction; // number of bits used for encoding an instruction
  double decode_time_per_instruction;
  double t1; // thermal relaxation time
  bool   stats_detailed;
  
  
  Parameters() {}

  void display() const;
  void displayGateDelays() const;

  bool readFromFile(const string& file_name);

  void updateEPRDelay(const double nv);
  void updateDistDelay(const double nv);
  void updatePreDelay(const double nv);
  void updatePostDelay(const double nv);
  void updateNoCClockTime(const double nv);
  void updateWBitRate(const double nv);
  void updateTokenPassTime(const double nv);
  void updateMemoryBandwidth(const double nv);
  void updateBitsInstruction(const int nv);
  void updateDecodeTime(const double nv);
  void updateThermalRelaxationTime(const double nv);
  void updateStatsDetailed(const bool nv);

};

#endif
