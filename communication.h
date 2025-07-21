// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: communication.h
// Description: Declaration of communication structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================


#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <list>

using namespace std;

struct Communication
{
  int src_core;
  int dst_core;
  int volume;
  
  Communication(int src, int dst, int vol) : src_core(src), dst_core(dst), volume(vol) {}

  bool operator<(const Communication& other) const;
  
  void display() const;
};

typedef list<Communication> ParallelCommunications;

void displayParallelCommunications(const ParallelCommunications& pc);
int getTotalCommunicationVolume(const ParallelCommunications& pc);

#endif

