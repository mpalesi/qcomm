#include <iostream>
#include <fstream>
#include <sstream>
#include "parameters.h"

void Parameters::display() const
{
  cout << endl
       << "*** Parameters ***" << endl
       << "gate_delay: " << gate_delay << endl
       << "hop_delay: " << hop_delay << endl
       << "epr_delay: " << epr_delay << endl
       << "dist_delay: " << dist_delay << endl
       << "pre_delay: " << pre_delay << endl
       << "post_delay: " << post_delay << endl;
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
      else {
	cout << "Invalid patameter reading " << file_name
	     << ": '" << param << "'" << endl;
	return(false);
      }
    }

  input_file.close();
  
  return true;
}
