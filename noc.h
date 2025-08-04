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

  // token_owner_map[rc] gives the core_id enabled to use the radio
  // channel rc
  mutable vector<int> token_owner_map; 
  
  NoC() {}


  // Initialize token_owner_map (tom) in such a way tokens are equally
  // distributed among the cores
  // Examples: 4 cores, 1 radio channel: tom[0] = 0
  //           4 cores, 2 radio channels: tom[0] = 0, tom[1] = 2
  //           4 cores, 3 radio channels: tom[s0] = 0, tom[1] = 1, tom[2] = 2
  void initializeTokenOwnerMap();

  // Returns the first available radio channel for src_node, -1 if
  // radio channels are not available for src_node
  int getRadioChannel(const int src_node) const;

  // Advances the tokens to the next WI. For each WI, update the
  // timeline of i-th WI by adding a time equal to the transmission
  // time used by i-th WI for transmitting its message
  void advanceTokensAndUpdateTimeLine(vector<double>& timeline) const;
  
  void enableWiNoC(const double _bit_rate, const int _radio_channels, double _token_pass_time);

  // Display NoC/WiNoC information to the stdout in YAML format
  void display();

  // Computes the communication time for the set of parallel
  // communications. This is the main function that invokes the
  // appropriate getCommunicationTime method for NoC or WiNoC
  double getCommunicationTime(const ParallelCommunications& pc) const;

  // Computes the communication time for the set of parallel
  // communications for the wired NoC.
  double getCommunicationTimeWired(const ParallelCommunications& pc) const;

  // Computes the communication time for the set of parallel
  // communications for the WiNoC. This is the main function which
  // calls the appropriate getCommunicationTimeWireless based on the
  // selected MAC
  double getCommunicationTimeWireless(const ParallelCommunications& pc) const; 

  // Simulate a token-passing mechanism. There are as many tokens as
  // there are radio channels. The tokens circulate sequentially among
  // the wireless interfaces (WIs). A WI holding a token is allowed to
  // use the corresponding radio channel and may retain it for the
  // entire duration of its transmission. After the transmission, the
  // token is passed to the next WI in the sequence. The act of
  // passing the token introduces a delay, referred to as
  // token_pass_time.
  double getCommunicationTimeWirelessToken(const ParallelCommunications& pc) const; 

  // Assign transmissions to channels to minimize the makespan (i.e.,
  // the time when the last channel finishes).  Longest Processing Time
  // First (LPT) heuristic Sort comms in descending duration
  // (transmission time), then assign to channel with min current load
  double getCommunicationTimeWirelessLTP(const ParallelCommunications& pcomms) const; 

  // XY routing algorithm
  int routingXY(const int src_core, const int dst_core) const;

  // Returns x and y coordinates of core core_id into the 2D mesh
  void getCoreXY(const int core_id, int& x, int& y) const;

  // Returns the core id of the core located on coordinates x and y in
  // the 2D mesh
  int getCoreID(const int x, const int y) const;

  // Returns the number of cycles spent to transfer volume bits in a
  // NoC link
  int linkTraversalCycles(int volume) const;

  // Returns the time to transfers volume bits in a NoC link or in a
  // radio channel based on the value of winoc attribute.
  double getTransferTime(int volume) const;

  // This method is used by getCommunicationTimeWired for assigning a
  // different id to each communication.
  map<int,Communication> assignCommunicationIds(const ParallelCommunications& pcomms) const;

  // This method is used by updateLinkOccupation which in turn is used
  // by getCommunicationTimeWired for inferring whether a specific
  // communication is stil in queue for transmission
  bool commIsInQueue(const int id, const queue<pair<int,int> >& qc) const;

  
  // This method is used by updateLinkOccupation which in turn is used
  // by getCommunicationTimeWired for computing the start time of the
  // communication which correspond to the time when the other
  // communications in the queue before it are served
  int computeStartTime(const queue<pair<int,int> >& qc) const;

  // This method is used by getCommunicationTimeWired to update the
  // status of the NoC link in terms of their occupancy at a specific
  // clock cycle
  bool updateLinksOccupation(map<pair<int,int>, queue<pair<int,int> > >& links_occupation,
			     const int cid, Communication& comm,
			     const int clock_cycle) const;

  // This method is used by getCommunicationTimeWired do determine the
  // next clock cycle which is computed as the minimum waiting time in
  // the links_occupation structure
  int nextClockCycle(const map<pair<int,int>, queue<pair<int,int> > >& links_occupation) const;
};

#endif
