#ifndef __NOC_H__
#define __NOC_H__

#include <map>
#include "communication.h"

using namespace std;

struct NoC
{
  int mesh_x, mesh_y;
  
  NoC(int _mesh_x, int _mesh_y) : mesh_x(_mesh_x), mesh_y(_mesh_y) {}

  double getCommunicationTime(const ParallelCommunications& pc, double hop_time);
  void updateCommunication(Communication& comm, map<pair<int,int>, int>& links,
			   int time_step, bool& drained);
  int routingXY(const int src_core, const int dst_core);
  void getCoreXY(const int core_id, int& x, int& y);
  int getCoreID(const int x, const int y);
};

#endif
