#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include <string>

using namespace std;

struct Parameters
{
  double gate_delay;
  double hop_delay;
  double epr_delay;
  double dist_delay;
  double pre_delay;
  double post_delay;

  Parameters() : gate_delay(0.0), hop_delay(0.0), epr_delay(0.0), dist_delay(0.0), pre_delay(0.0), post_delay(0.0) {}

  void display() const;

  bool readFromFile(const string& file_name);

  void updateHopDelay(const double nv);
};

#endif
