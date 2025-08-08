// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: teleportation_time.h
// Description: Declaration of teleportation_time structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __TELEPORTATION_TIME_H__
#define __TELEPORTATION_TIME_H__

#include <string>
using namespace std;

struct TeleportationTime
{
  double t_epr; // EPR pair generation time
  double t_dist; // EPR pair distribution time
  double t_pre; // Pre-processing time
  double t_clas; // Classical transfer time
  double t_post; // Post-processing time

  TeleportationTime() : t_epr(0.0), t_dist(0.0), t_pre(0.0), t_clas(0.0), t_post(0.0) {}

  void display(const string& prefix = "") const;

  double getTotalTeleportationTime() const;
};

#endif
