#include <iostream>
#include "noc.h"

double NoC::getCommunicationTime(const ParallelCommunications& pcomms, double hop_time)
{
  map<pair<int,int>, int> links; // link[(node1,node2)] --> timestep when link is used (busy)

  int time_step = 0;
  bool traffic_drained;
  ParallelCommunications lpcomms = pcomms;
  
  do {
    traffic_drained = true;
    ParallelCommunications new_pcomms;
    for (auto comm : lpcomms)
      {
	bool drained;
	updateCommunication(comm, links, time_step, drained);
	new_pcomms.push_back(comm);
	if (!drained)
	  traffic_drained = false;
      }
    time_step++;
    lpcomms = new_pcomms;
  } while (!traffic_drained);

  // search for the maximum time_step in the links
   int max= std::numeric_limits<int>::min();
   for (const auto& link : links)
     if (link.second > max)
       max = link.second;
        
   return (max+1) * hop_time;
}

void NoC::updateCommunication(Communication& comm,
			      map<pair<int,int>, int>& links,
			      int time_step, bool& drained)
{
  if (comm.src_core == comm.dst_core)
    drained = true;
  else
    {
      drained = false;
      
      int next_core = routingXY(comm.src_core, comm.dst_core);
      
      pair<int,int> link(comm.src_core, next_core);
      auto it = links.find(link);
      if (it == links.end() || it->second != time_step)
	{
	  links[link] = time_step;
	  comm.src_core = next_core;
	}
    }
}

int NoC::routingXY(const int src_core, const int dst_core)
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

void NoC::getCoreXY(const int core_id, int& x, int& y)
{
  x = core_id % mesh_x;
  y = core_id / mesh_x;
}

int NoC::getCoreID(const int x, const int y)
{
  return y * mesh_x + x;
}
