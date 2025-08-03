// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: noc.h
// Description: Declaration of noc structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __NOC_H__
#define __NOC_H__

#include <map>
#include <queue>
#include <vector>
#include "communication.h"

using namespace std;

#define WIRELESS_MAC_TOKEN 0
#define WIRELESS_MAC_LPT   1

struct NoC
{
  int    mesh_x, mesh_y;
  int    link_width;
  double clock_time;
  int    qubit_addr_bits;
  double wbit_rate;
  int    radio_channels;
  double token_pass_time;
  int    wireless_mac;
  bool   winoc;
  mutable vector<int> token_owner_map; // token_owner_map[rc] gives the core_id
			       // enabled to use the radio channel rc

  
  NoC() {}

  
  void initializeTokenOwnerMap();

  // returns the first available radio channel for src_node, -1 if
  // radio channels are availble for src_node
  int getRadioChannel(const int src_node) const;
  void advanceTokensAndUpdateTimeLine(vector<double>& timeline) const;
  
  void enableWiNoC(const double _bit_rate, const int _radio_channels, double _token_pass_time);
  
  void display();
    
  double getCommunicationTime(const ParallelCommunications& pc) const;

  double getCommunicationTimeWired(const ParallelCommunications& pc) const;

  // Main wireless MAC which calls the appropriate wireless MAC
  double getCommunicationTimeWireless(const ParallelCommunications& pc) const; 

  // Simulate a token-passing mechanism. There are as many tokens as
  // there are radio channels. The tokens circulate sequentially among
  // the wireless interfaces (WIs). A WI holding a token is allowed to
  // use the corresponding radio channel and may retain it for the
  // entire duration of its transmission. After the transmission, the
  // token is passed to the next WI in the sequence. The act of
  // passing the token introduces a delay, referred to as
  // token_pass_time.
  double getCommunicationTimeWirelessToken(const ParallelCommunications& pc) const; // standard token based MAC

  // Assign transmissions to channels to minimize the makespan (i.e.,
  // the time when the last channel finishes).  Longest Processing Time
  // First (LPT) heuristic Sort comms in descending duration
  // (transmission time), then assign to channel with min current load
  double getCommunicationTimeWirelessLTP(const ParallelCommunications& pcomms) const; // LPT based MAC

  int routingXY(const int src_core, const int dst_core) const;
  void getCoreXY(const int core_id, int& x, int& y) const;
  int getCoreID(const int x, const int y) const;

  int linkTraversalCycles(int volume) const;
  double getTransferTime(int volume) const;
  map<int,Communication> assignCommunicationIds(const ParallelCommunications& pcomms) const;
  bool commIsInQueue(const int id, const queue<pair<int,int> >& qc) const;
  int computeStartTime(const queue<pair<int,int> >& qc) const;
  bool updateLinksOccupation(map<pair<int,int>, queue<pair<int,int> > >& links_occupation,
			     const int cid, Communication& comm,
			     const int clock_cycle) const;
  int nextClockCycle(const map<pair<int,int>, queue<pair<int,int> > >& links_occupation) const;

};

#endif
