// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: architecture.cpp
// Description: Implementation of architecture and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include "utils.h"
#include "architecture.h"
#include "mapping.h"

void Architecture::display() const
{
  cout << endl
       << "Architecture:" << endl
       << IND << "number_of_cores: " << number_of_cores << endl
       << IND << "qubits_per_core: " << qubits_per_core << endl
       << IND << "total_physical_qubits: " << total_physical_qubits << endl
       << IND << "ltm_ports: " << ltm_ports << endl;

  cout << IND << "teleportation_type: " << teleportation_type;
  if (teleportation_type == TP_TYPE_A2A)
    cout << " # all to all" << endl;
  else if (teleportation_type == TP_TYPE_MESH)
    cout << " # mesh" << endl;
  else 
    cout << " # ??\?" << endl;

  cout << IND << "dst_selection_mode: " << dst_selection_mode;
  if (dst_selection_mode == DST_SEL_LOAD_INDEPENDENT)
    cout << " # load independent" << endl;
  else if (dst_selection_mode == DST_SEL_LOAD_AWARE)
    cout << " # load aware" << endl;
  else
    cout << " # ??\?" << endl;
  
  
  cout << IND << "mapping_type: " << mapping_type;
  if (mapping_type == MAP_RANDOM)
    cout << " # random" << endl;
  else if (mapping_type == MAP_SEQUENTIAL)
    cout << " # sequential" << endl;
  else
    cout << " # ??\?" << endl;

  noc.display();
  cores.display();
}

bool Architecture::readFromFile(const string& file_name)
{
  YAML::Node config;
  bool result = loadYAMLFile(file_name, config);
  
  result &= getOrFail<int>(config, "mesh_x", file_name, noc.mesh_x);
  result &= getOrFail<int>(config, "mesh_y", file_name, noc.mesh_y);
  result &= getOrFail<int>(config, "link_width", file_name, noc.link_width);
  result &= getOrFail<int>(config, "qubits_per_core", file_name, qubits_per_core);
  result &= getOrFail<int>(config, "ltm_ports", file_name, ltm_ports);
  result &= getOrFail<int>(config, "radio_channels", file_name, noc.radio_channels);
  result &= getOrFail<bool>(config, "wireless_enabled", file_name, noc.winoc);
  result &= getOrFail<int>(config, "teleportation_type", file_name, teleportation_type);
  result &= getOrFail<int>(config, "wireless_mac", file_name, noc.wireless_mac);
  result &= getOrFail<int>(config, "dst_selection_mode", file_name, dst_selection_mode);
  result &= getOrFail<int>(config, "mapping_type", file_name, mapping_type);
  
  computeDerivedVariables();
  
  return result;
}

void Architecture::computeDerivedVariables()
{
  number_of_cores = noc.mesh_x * noc.mesh_y;
  total_physical_qubits = number_of_cores * qubits_per_core;

  noc.qubit_addr_bits = ceil(log2(qubits_per_core * number_of_cores));  
}

void Architecture::updateMeshX(const int nv)
{
  noc.mesh_x = nv;
  computeDerivedVariables();
}

void Architecture::updateMeshY(const int nv)
{
  noc.mesh_y = nv;
  computeDerivedVariables();
}

void Architecture::updateLinkWidth(const int nv)
{
  noc.link_width = nv;
}

void Architecture::updateQubitsPerCore(const int nv)
{
  qubits_per_core = nv;
  computeDerivedVariables();
}

void Architecture::updateLTMPorts(const int nv)
{
  ltm_ports = nv;
}

void Architecture::updateRadioChannels(const int nv)
{
  noc.radio_channels = nv;
}

void Architecture::updateWirelessEnabled(const int nv)
{
  noc.winoc = nv;
}

void Architecture::updateTeleportationType(const int nv)
{
  teleportation_type = nv;
}

void Architecture::updateWirelessMAC(const int nv)
{
  noc.wireless_mac = nv;
}

void Architecture::updateDstSelectionMode(const int nv)
{
  dst_selection_mode = nv;
}

void Architecture::updateMappingType(const int nv)
{
  mapping_type = nv;
}


