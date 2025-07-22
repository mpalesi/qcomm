// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: noc.cpp
// Description: Implementation of noc structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <cmath>
#include <cassert>
#include <algorithm>
#include "utils.h"
#include "noc.h"

NoC::NoC(int _mesh_x, int _mesh_y, int _link_width, double _clock_time, int _qubit_addr_bits)
{
  winoc = false;
  
  mesh_x = _mesh_x;
  mesh_y = _mesh_y;
  link_width = _link_width;
  clock_time = _clock_time;
  qubit_addr_bits = _qubit_addr_bits;
}

void NoC::enableWiNoC(const double _bit_rate, const int _radio_channels, double _token_pass_time)
{
  winoc = true;

  wbit_rate = _bit_rate;
  radio_channels = _radio_channels;
  token_pass_time = _token_pass_time;

  initializeTokenOwnerMap();
}

void NoC::display()
{
  if (!winoc)
    {
      cout << endl
	   << "NoC:" << endl
	   << IND << "mesh_size: '" << mesh_x << "x" << mesh_y << "'" << endl
	   << IND << "clock_period: " << clock_time << " # sec" << endl
	   << IND << "link_width: " << link_width << " # bits" << endl;
    }
  else
    {
      cout << endl
	   << "WiNoC:" << endl
	   << IND << "bit_rate: " << wbit_rate << " # bps" << endl
	   << IND << "radio_channels: " << radio_channels << endl
	   << IND << "token_pass_time: " << token_pass_time << " # sec" << endl;
    }
}


map<int,Communication>  NoC::assignCommunicationIds(const ParallelCommunications& pcomms) const
{
  map<int,Communication> pcomms_id;
  int id = 0;

  for (Communication comm : pcomms)
    pcomms_id.insert({id++, comm});
  
  return pcomms_id;
}

bool NoC::commIsInQueue(const int id, const queue<pair<int,int> >& qc) const
{
  queue<pair<int,int> > temp = qc;

  while (!temp.empty())
    {
      if (temp.front().first == id)
	return true;

      temp.pop();
    }
  
  return false;
}

int NoC::computeStartTime(const queue<pair<int,int>, deque<pair<int,int>>>& qc) const
{
  /*
  int st = 0;
  queue<pair<int,int> > temp = qc;

  while (!temp.empty())
    {
      // st += temp.front().second;
      st = temp.front().second; 
      temp.pop();
    }

  return st;
  */
  
  return qc.back().second;
}

int NoC::nextClockCycle(const map<pair<int,int>, queue<pair<int,int> > >& links_occupation) const
{
  // the next clock cycle corresponds to the minimum waiting time in the links_occupation structure

  int min_cc = std::numeric_limits<int>::max();
  for (const auto& l : links_occupation)
    {
      queue<pair<int,int> > q = l.second;
      while (!q.empty())
	{
	  int cc = q.front().second;
	  q.pop();
	  
	  if (cc < min_cc)
	    min_cc = cc;
	}
    }

  return min_cc;
}

bool NoC::updateLinksOccupation(map<pair<int,int>, queue<pair<int,int>, deque<pair<int,int>>>>& links_occupation,
				const int cid, Communication& comm,
				const int clock_cycle) const
{
  bool drained = false;
  
  int next_core = routingXY(comm.src_core, comm.dst_core);

  pair<int,int> link(comm.src_core, next_core);

  auto it_links_occupation = links_occupation.find(link);

  if (it_links_occupation == links_occupation.end())
    {
      // comm_id is the first one traversing the link
      queue<pair<int,int> > q;
      q.push(pair<int,int>(cid, clock_cycle + linkTraversalCycles(comm.volume)));
      links_occupation[link] = q;
    }
  else
    {
      // link is already used
      if (!commIsInQueue(cid, it_links_occupation->second))
	{
	  // if the link exists then the queue must contains communications otherwise
	  // the link is removed from the links_occupation structure
	  assert(!it_links_occupation->second.empty());

	  // if comm is not yet in queue for the link, add it in queue. It will start when all the
	  // previous communications left the link
	  it_links_occupation->second.push(pair<int,int>(cid,
							 computeStartTime(it_links_occupation->second) +
							 linkTraversalCycles(comm.volume)));	  
	}
      else
	{
	  // communication is in queue. If it is in the front and its
	  // release time passed than it can advance and removed from
	  // the queue
	  if (it_links_occupation->second.front().first == cid &&
	      clock_cycle >= it_links_occupation->second.front().second)
	    {
	      comm.src_core = next_core;

	      // check if drained
	      if (comm.src_core == comm.dst_core)
		drained = true;

	      // remove the communication from the queue of the current link
	      it_links_occupation->second.pop();

	      // check if the queue is empty and in this case remove the link from links_occupation
	      if (it_links_occupation->second.empty())
		links_occupation.erase(it_links_occupation);
	    }
	}
    }

  return drained;
}

double NoC::getCommunicationTimeWired(const ParallelCommunications& pcomms) const
{
  map<pair<int,int>, queue<pair<int,int>, deque<pair<int,int>>>> links_occupation; // links_occupation[(node1,node2)] --> queue of pairs (comm_id, when the link is released)
  map<int,Communication> pcomms_id = assignCommunicationIds(pcomms);
  int clock_cycle = 0;
  
  while (!pcomms_id.empty())
    {      
      // DEBUG      cout << endl << endl << "cc: " << clock_cycle << endl;
      
      for (map<int,Communication>::iterator it = pcomms_id.begin(); it != pcomms_id.end(); )
	{
	  bool drained = updateLinksOccupation(links_occupation, it->first, it->second, clock_cycle);
	  /* DEBUG
	  cout << "\tcomm id " << it->first << ": " << it->second.src_core << "-->" << it->second.dst_core << " (" << it->second.volume << ")" << endl;
	  cout << "\tlinks occupation:" << endl;
	  
	  for (const auto& lo : links_occupation)
	    {
	      cout << "\t" << lo.first.first << "-->" << lo.first.second << ": ";
	      queue<pair<int,int> > q = lo.second;
	      while (!q.empty())
		{
		  cout << "(" << q.front().first << "," << q.front().second << "), ";
		  q.pop();
		}
	      cout << endl;
	    }
	  */
	  
	  if (drained)
	    it = pcomms_id.erase(it);
	  else
	    ++it;
	}

      if (!links_occupation.empty())
	clock_cycle = nextClockCycle(links_occupation);
    }

  return clock_cycle * clock_time;
}

void NoC:: initializeTokenOwnerMap()
{
  // Initialize token_owner_map (tom) in such a way tokens are equally
  // distributed among the cores
  // Examples: 4 cores, 1 radio channel: tom[0] = 0
  //           4 cores, 2 radio channels: tom[0] = 0, tom[1] = 2
  //           4 cores, 3 radio channels: tom[s0] = 0, tom[1] = 1, tom[2] = 2
  token_owner_map.resize(radio_channels);
  int ncores = mesh_x * mesh_y;
  int step = ncores / radio_channels;
  int core_id = 0;
  for (int rc=0; rc < radio_channels; rc++)
    {
      token_owner_map[rc] = core_id;
      core_id += (step % ncores);
    }
}


int NoC::getRadioChannel(const int src_node) const
{
  for (int rc=0; rc < radio_channels; rc++)
    if (token_owner_map[rc] == src_node)
      return rc;

  return -1;
}

void NoC::advanceTokensAndUpdateTimeLine(vector<double>& timeline) const
{
  for (int rc=0; rc < radio_channels; rc++)
    {
      // advance token to the next core in sequence
      token_owner_map[rc] = (token_owner_map[rc] + 1) % (mesh_x * mesh_y);

      // update the timeline with token_pass_time
      timeline[rc] += token_pass_time;
    }
}

double NoC::getCommunicationTimeWireless(const ParallelCommunications& pcomms) const
{
  // Simulate a token-passing mechanism. There are as many tokens as
  // there are radio channels. The tokens circulate sequentially among
  // the wireless interfaces (WIs). A WI holding a token is allowed to
  // use the corresponding radio channel and may retain it for the
  // entire duration of its transmission. After the transmission, the
  // token is passed to the next WI in the sequence. The act of
  // passing the token introduces a delay, referred to as
  // token_pass_time.
  vector<double> timeline(radio_channels, 0.0);
  ParallelCommunications pc = pcomms;
  while (!pc.empty())
    {
      ParallelCommunications::iterator curr = pc.begin();
      
      do {
	int src_core = curr->src_core;
	int rc = getRadioChannel(src_core);

	if (rc == -1)
	  {
	    // No radio channel available for src_core, continue with
	    // next communciation;
	    curr++;
	  }
	else {
	  // update the timeline associated to radio channel rc
	  timeline[rc] += getTransferTime(curr->volume);

	  // remove communication from the list of parallel
	  // communications
	  curr = pc.erase(curr);
	}
      } while (curr != pc.end());

      advanceTokensAndUpdateTimeLine(timeline);
    }

  // The communication time is the maximum among the timelines
  auto max_it = max_element(timeline.begin(), timeline.end());
  assert(max_it != timeline.end());
  return *max_it;
}

 

double NoC::getCommunicationTime(const ParallelCommunications& pcomms) const
{
  if (!winoc)
    return getCommunicationTimeWired(pcomms);
  else
    return getCommunicationTimeWireless(pcomms);  
}

double NoC::getTransferTime(int volume) const
{
  if (!winoc)
    return volume / (link_width / clock_time);
  else
    return volume / wbit_rate;
}

int NoC::linkTraversalCycles(int volume) const
{
  return ceil((double)volume/link_width);
}

int NoC::routingXY(const int src_core, const int dst_core) const
{
  int x, y, xd, yd;
  
  getCoreXY(src_core, x, y);
  getCoreXY(dst_core, xd, yd);

  if (x < xd)
    x++;
  else if (x > xd)
    x--;
  else if (y < yd)
    y++;
  else if (y > yd)
    y--;

  return getCoreID(x, y);
}

void NoC::getCoreXY(const int core_id, int& x, int& y) const
{
  x = core_id % mesh_x;
  y = core_id / mesh_x;
}

int NoC::getCoreID(const int x, const int y) const
{
  return y * mesh_x + x;
}
