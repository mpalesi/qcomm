#include <iostream>
#include "communication_time.h"

using namespace std;

void CommunicationTime::display() const
{
  cout << "communication_time: " << getTotalTime() << endl
       << "\tt_epr: " << t_epr << endl
       << "\tt_dist: " << t_dist << endl
       << "\tt_pre: " << t_pre << endl
       << "\tt_clas: " << t_clas << endl
       << "\tt_post: " << t_post << endl;
}

double CommunicationTime::getTotalTime() const
{
  return t_epr + t_dist + t_pre + t_clas + t_post;
}
