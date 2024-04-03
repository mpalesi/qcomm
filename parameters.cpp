#include <iostream>
#include <fstream>
#include <sstream>
#include "parameters.h"

void Parameters::display() const
{
  cout << endl
       << "*** Parameters ***" << endl
       << "gate delay (s): " << gate_delay << endl
       << "hop delay (s): " << hop_delay << endl
       << "epr delay (s): " << epr_delay << endl
       << "dist delay (s): " << dist_delay << endl
       << "pre delay (s): " << pre_delay << endl
       << "post delay (s): " << post_delay << endl
       << "wbit rate (bps): " << wbit_rate << endl
       << "token pass time (s): " << token_pass_time << endl;
}

bool Parameters::readFromFile(const string& file_name)
{
  ifstream input_file(file_name);
  if (!input_file.is_open())
    return(false);

  string line;
  while (getline(input_file, line))
    {
      istringstream iss(line);
      string param;
      
      iss >> param;
      if (param == string("gate_delay"))
	iss >> gate_delay;
      else if (param == string("hop_delay"))
	iss >> hop_delay;
      else if (param == string("epr_delay"))
	iss >> epr_delay;
      else if (param == string("dist_delay"))
	iss >> dist_delay;
      else if (param == string("pre_delay"))
	iss >> pre_delay;
      else if (param == string("post_delay"))
	iss >> post_delay;
      else if (param == string("wbit_rate"))
	iss >> wbit_rate;
      else if (param == string("token_pass_time"))
	iss >> token_pass_time;
      else {
	cout << "Invalid patameter reading " << file_name
	     << ": '" << param << "'" << endl;
	return(false);
      }
    }

  input_file.close();
  
  return true;
}

void Parameters::updateHopDelay(const double nv)
{
  hop_delay = nv;
}

void Parameters::updateEPRDelay(const double nv)
{
  epr_delay = nv;
}

void Parameters::updatePreDelay(const double nv)
{
  pre_delay = nv;
}

void Parameters::updatePostDelay(const double nv)
{
  post_delay = nv;
}

void Parameters::updateWBitRate(const double nv)
{
  wbit_rate = nv;
}

void Parameters::updateTokenPassTime(const double nv)
{
  token_pass_time = nv;
}
