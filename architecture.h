#ifndef __ARCHITECTURE_H__
#define __ARCHITECTURE_H__

#include <string>

using namespace std;

struct Architecture
{
  int  mesh_x, mesh_y;
  int  link_width;
  int  qubits_per_core;
  int  ltm_ports;
  int  number_of_cores; // derived value
  bool configured;
  
  Architecture() : configured(false) {}

  void display() const;
  
  bool readFromFile(const string& file_name);

  void updateMeshX(const int nv);
  void updateMeshY(const int nv);
  void updateLinkWidth(const int nv);
  void updateQubitsPerCore(const int nv);
  void updateLTMPorts(const int nv);

  void updateDerivedVariables();
};

#endif
