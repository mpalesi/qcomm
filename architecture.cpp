#include <iostream>
#include <fstream>
#include <sstream>
#include "architecture.h"


void Architecture::display() const
{
  cout << endl
       << "*** Architecture ***" << endl
       << "mesh_x x mesh_y: " << mesh_x << "x" << mesh_y << endl
       << "qubits_per_core: " << qubits_per_core
       << " (total physical qubits: " << mesh_x * mesh_y * qubits_per_core << ")" << endl
       << "ltm_ports = " << ltm_ports
       << endl;
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
      else if (attribute == string("qubits_per_core"))
	iss >> qubits_per_core;
      else if (attribute == string("ltm_ports"))
	iss >> ltm_ports;
      else {
	cout << "Invalid attribute reading " << file_name
	     << ": '" << attribute << "'" << endl;
	return false;
      }
    }

  input_file.close();

  number_of_cores = mesh_x * mesh_y;

  configured = true;

  return true;
}

void Architecture::updateMeshX(const int nv)
{
  mesh_x = nv;
  number_of_cores = mesh_x * mesh_y;
}

void Architecture::updateMeshY(const int nv)
{
  mesh_y = nv;
  number_of_cores = mesh_x * mesh_y;
}

void Architecture::updateQubitsPerCore(const int nv)
{
  qubits_per_core = nv;
}

void Architecture::updateLTMPorts(const int nv)
{
  ltm_ports = nv;
}
