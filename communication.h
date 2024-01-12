#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <set>

using namespace std;

struct Communication
{
  int src_core;
  int dst_core;

  Communication(int src, int dst) : src_core(src), dst_core(dst) {}

  bool operator<(const Communication& other) const;
  
  void display() const;
};

typedef set<Communication> ParallelCommunications;

void displayParallelCommunications(const ParallelCommunications& pc);

#endif

