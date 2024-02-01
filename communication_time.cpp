#include <iostream>
#include <iomanip>
#include "communication_time.h"

using namespace std;

void CommunicationTime::display() const
{
  double total_time = getTotalTime();
  
  cout << "Communication_time (s): " << total_time << endl
       << "\tEPR pair generation time (s): " << t_epr << " (" << fixed << 100*t_epr/total_time << "%)" << endl
       << "\tEPR pair distribution time (s): " << t_dist << " (" << fixed << 100*t_dist/total_time << "%)" << endl
       << "\tPre-processing time (s): " << t_pre << " (" << fixed << 100*t_pre/total_time << "%)" << endl
       << "\tClassical transfer time (s): " << t_clas << " (" << fixed << 100*t_clas/total_time << "%)" << endl
       << "\tPost-processing time (s): " << t_post << " (" << fixed << 100*t_post/total_time << "%)" << endl;
}

double CommunicationTime::getTotalTime() const
{
  return t_epr + t_dist + t_pre + t_clas + t_post;
}
