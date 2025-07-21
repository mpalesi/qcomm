// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: communication.cpp
// Description: Implementation of communication structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include "communication.h"


bool Communication::operator<(const Communication& other) const
{
  if (src_core < other.src_core) {
    return true;
  } else if (src_core == other.src_core) {
    return dst_core < other.dst_core;
  } else {
    return false;
  }
}

void Communication::display() const
{
  cout << src_core << "->" << dst_core << "(" << volume << ")";
}

void displayParallelCommunications(const ParallelCommunications& pc)
{
  for (const auto& comm : pc)
    {
      comm.display();
      cout << " ";
    }
  cout << endl;
}

int getTotalCommunicationVolume(const ParallelCommunications& pc)
{
  int volume = 0;

  for (Communication comm : pc)
    volume += comm.volume;

  return volume;
}
