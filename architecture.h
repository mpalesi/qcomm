// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: architecture.h
// Description: Declaration of architecture structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __ARCHITECTURE_H__
#define __ARCHITECTURE_H__

#include <string>
#include "noc.h"
#include "core.h"

using namespace std;

#define TP_TYPE_A2A  0
#define TP_TYPE_MESH 1

#define DST_SEL_LOAD_INDEPENDENT 0
#define DST_SEL_LOAD_AWARE       1


struct Architecture
{
  int     qubits_per_core;
  int     ltm_ports;
  int     number_of_cores; // derived value
  int     total_physical_qubits; // derived value
  int     teleportation_type;
  int     dst_selection_mode;
  int     mapping_type;

  Cores&  cores;
  NoC&    noc;
  
  Architecture(Cores& cores_ref, NoC& noc_ref) : cores(cores_ref), noc(noc_ref) {}
  
  void display() const;

  // Read the architecture from a YAML file. Returns true if success,
  // 0 otherwise
  bool readFromFile(const string& file_name); 

  void updateMeshX(const int nv);
  void updateMeshY(const int nv);
  void updateLinkWidth(const int nv);
  void updateQubitsPerCore(const int nv);
  void updateLTMPorts(const int nv);
  void updateRadioChannels(const int nv);
  void updateWirelessEnabled(const int nv);
  void updateTeleportationType(const int nv);
  void updateWirelessMAC(const int nv);
  void updateDstSelectionMode(const int nv);
  void updateMappingType(const int nv);

  void updateDerivedVariables();
};

#endif
