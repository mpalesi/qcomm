#include <iostream>
#include <cmath>
#include "noc.h"

NoC::NoC(int _mesh_x, int _mesh_y, int _link_width, double _hop_time)
{
  mesh_x = _mesh_x;
  mesh_y = _mesh_y;
  link_width = _link_width;
  hop_time = _hop_time;

  int address_size = ceil(log2(mesh_x * mesh_y));
 
  packet_size = 2 + 2*address_size; // 2 bits per teleportation
				    // protocol, 2*address_size bit
				    // for src and dst
  cycles_per_packet = ceil((double)packet_size / link_width);
}


void NoC::display()
{
  cout << endl
       << "*** NoC ***" << endl
       << "mesh_x x mesh_y: " << mesh_x << "x" << mesh_y << endl
       << "link width (bits): " << link_width << endl
       << "packet_size (bits): " << packet_size << endl
       << "cycles per packets: " << cycles_per_packet
       << endl;
}


double NoC::getCommunicationTime(const ParallelCommunications& pcomms) const
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
        
   return (max+1) * hop_time * cycles_per_packet;
}

void NoC::updateCommunication(Communication& comm,
			      map<pair<int,int>, int>& links,
			      int time_step, bool& drained) const
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


double NoC::getThroughput(int ncomms, double etime) const
{
  return ncomms * packet_size / etime;
}
