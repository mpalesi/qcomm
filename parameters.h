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
#include "noc.h"

using namespace std;

struct Parameters
{
  map<string,double> gate_delays; // <gate name, delay>
  double epr_delay;
  double dist_delay;
  double pre_delay;
  double post_delay;
  double memory_bandwidth; // bits/sec
  int    bits_instruction; // number of bits used for encoding an instruction
  double decode_time_per_instruction;
  double t1; // thermal relaxation time
  bool   stats_detailed;

  // quantum scaling factor: all the quantum related parameters are
  // multiplied by qscale_factor before being used. This is useful to
  // simpplify the analysis of technological trends
  // NOTE: It does not affect t1
  double qscale_factor;

  NoC&   noc;
  
  Parameters(NoC& noc_ref) : noc(noc_ref) {}

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
  void updateQScaleFactor(const double nv);

  // update the quantum related parameters by taking into account the
  // qscale_factor
  void scaleQuantumRelatedParameters();
};

#endif
