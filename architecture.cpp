#include <iostream>
#include <fstream>
#include <sstream>
#include "architecture.h"


void Architecture::display() const
{
  cout << endl
       << "*** Architecture ***" << endl
       << "mesh_x x mesh_y: " << mesh_x << "x" << mesh_y << endl
       << "qubits per core: " << qubits_per_core
       << " (total physical qubits: " << mesh_x * mesh_y * qubits_per_core << ")" << endl
       << "ltm ports = " << ltm_ports << endl;

  if (wireless_enabled)
    cout << "wireless enabled" << endl
	 << "radio_channels: " << radio_channels << endl;
}

bool Architecture::readFromFile(const string& file_name)
{

  ifstream input_file(file_name);
  if (!input_file.is_open())
    return false;

  string line;
  while (getline(input_file, line))
    {
      istringstream iss(line);
      string attribute;
      
      iss >> attribute;
      if (attribute == string("mesh_x"))
	iss >> mesh_x;
      else if (attribute == string("mesh_y"))
	iss >> mesh_y;
      else if (attribute == string("link_width"))
	iss >> link_width;
      else if (attribute == string("qubits_per_core"))
	iss >> qubits_per_core;
      else if (attribute == string("ltm_ports"))
	iss >> ltm_ports;
      else if (attribute == string("radio_channels"))
	iss >> radio_channels;
      else if (attribute == string("wireless_enabled"))
	iss >> wireless_enabled;
      else {
	cout << "Invalid attribute reading " << file_name
	     << ": '" << attribute << "'" << endl;
	return false;
      }
    }

  input_file.close();

  updateDerivedVariables();
  
  configured = true;

  return true;
}

void Architecture::updateMeshX(const int nv)
{
  mesh_x = nv;
  updateDerivedVariables();
}

void Architecture::updateMeshY(const int nv)
{
  mesh_y = nv;
  updateDerivedVariables();
}

void Architecture::updateLinkWidth(const int nv)
{
  link_width = nv;
}

void Architecture::updateQubitsPerCore(const int nv)
{
  qubits_per_core = nv;
}

void Architecture::updateLTMPorts(const int nv)
{
  ltm_ports = nv;
}

void Architecture::updateRadioChannels(const int nv)
{
  radio_channels = nv;
}

void Architecture::updateWirelessEnabled(const int nv)
{
  wireless_enabled = nv;
}

void Architecture::updateDerivedVariables()
{
  number_of_cores = mesh_x * mesh_y;
}

