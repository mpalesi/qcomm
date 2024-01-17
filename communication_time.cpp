#include <iostream>
#include <iomanip>
#include "communication_time.h"

using namespace std;

void CommunicationTime::display() const
{
  double total_time = getTotalTime();
  
  cout << "communication_time: " << total_time << endl
       << "\tEPR pair generation time: " << t_epr << " (" << fixed << 100*t_epr/total_time << "%)" << endl
       << "\tEPR pair distribution time: " << t_dist << " (" << fixed << 100*t_dist/total_time << "%)" << endl
       << "\tPre-processing time: " << t_pre << " (" << fixed << 100*t_pre/total_time << "%)" << endl
       << "\tClassical transfer time: " << t_clas << " (" << fixed << 100*t_clas/total_time << "%)" << endl
       << "\tPost-processing time: " << t_post << " (" << fixed << 100*t_post/total_time << "%)" << endl;
}

double CommunicationTime::getTotalTime() const
{
  return t_epr + t_dist + t_pre + t_clas + t_post;
}
