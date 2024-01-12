#ifndef __ARCHITECTURE_H__
#define __ARCHITECTURE_H__

#include <string>

using namespace std;

struct Architecture
{
  int  mesh_x, mesh_y;
  int  qubits_per_core;
  int  ltm_ports;
  int  number_of_cores;
  bool configured;
  
  Architecture() : configured(false) {}

  void display() const;
  
  bool readFromFile(const string& file_name);
};

#endif
