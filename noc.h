#ifndef __NOC_H__
#define __NOC_H__

#include <map>
#include "communication.h"

using namespace std;

struct NoC
{
  int    mesh_x, mesh_y;
  int    link_width;
  double hop_time;
  int    packet_size;
  int    cycles_per_packet;

  NoC(int _mesh_x, int _mesh_y, int _link_width, double _hop_time);
  
  void display();
    
  double getCommunicationTime(const ParallelCommunications& pc) const;
  
  void updateCommunication(Communication& comm, map<pair<int,int>, int>& links,
			   int time_step, bool& drained) const;
  int routingXY(const int src_core, const int dst_core) const;
  void getCoreXY(const int core_id, int& x, int& y) const;
  int getCoreID(const int x, const int y) const;

  double getThroughput(int ncomms, double etime) const;
};

#endif
