#include <iostream>
#include <iomanip>
#include "communication_time.h"

using namespace std;

void CommunicationTime::display() const
{
  double total_time = getTotalTime();
  
  cout << "communication_time: " << scientific << setprecision(3) << total_time << endl
       << "\tt_epr: " << scientific << setprecision(3) << t_epr << " (" << fixed << setprecision(2) << 100*t_epr/total_time << "%)" << endl
       << "\tt_dist: " << scientific << setprecision(3) << t_dist << " (" << fixed << setprecision(2) << 100*t_dist/total_time << "%)" << endl
       << "\tt_pre: " << scientific << setprecision(3) << t_pre << " (" << fixed << setprecision(2) << 100*t_pre/total_time << "%)" << endl
       << "\tt_clas: " << scientific << setprecision(3) << t_clas << " (" << fixed << setprecision(2) << 100*t_clas/total_time << "%)" << endl
       << "\tt_post: " << scientific << setprecision(3) << t_post << " (" << fixed << setprecision(2) << 100*t_post/total_time << "%)" << endl;
}

double CommunicationTime::getTotalTime() const
{
  return t_epr + t_dist + t_pre + t_clas + t_post;
}
