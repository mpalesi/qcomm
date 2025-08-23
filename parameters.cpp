// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: parameters.cpp
// Description: Implementation of parameters structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <yaml-cpp/yaml.h>
#include "utils.h"
#include "parameters.h"

void Parameters::displayGateDelays() const
{
  cout << "{";

  for (auto it = gate_delays.begin(); it != gate_delays.end(); ++it)
    {
      cout << it->first << ": " << it->second;
      if (it != prev(gate_delays.end()))
	cout << ", ";
    }
    
  cout << "} # sec" << endl;
}

void Parameters::display() const
{
  cout << endl
       << "Parameters:" << endl
       << IND << "seed: " << seed << endl
       << IND << "qscale_factor: " << qscale_factor << endl
       << IND << "gate_delays: ";
  displayGateDelays();
  cout << IND << "epr_delay: " << epr_delay << " # sec" << endl
       << IND << "dist_delay: " << dist_delay << " # sec" << endl
       << IND << "pre_delay: " << pre_delay << " # sec" << endl
       << IND << "post_delay: " << post_delay << " # sec" << endl
       << IND << "memory_mandwidth: " << memory_bandwidth << " # bps" << endl
       << IND << "bits_instruction: " << bits_instruction << " # bits" << endl
       << IND << "decode_time_per_instruction: " << decode_time_per_instruction << " # sec" << endl
       << IND << "t1: " << t1 << " # sec" << endl;
}

bool Parameters::readFromFile(const string& file_name)
{
  YAML::Node config;
  bool result = loadYAMLFile(file_name, config);

  result &= getOrFail<map<string,double>>(config, "gate_delays", file_name, gate_delays);
  result &= getOrFail<double>(config, "epr_delay", file_name, epr_delay);
  result &= getOrFail<double>(config, "dist_delay", file_name, dist_delay);
  result &= getOrFail<double>(config, "pre_delay", file_name, pre_delay);
  result &= getOrFail<double>(config, "post_delay", file_name, post_delay);
  result &= getOrFail<double>(config, "noc_clock_time", file_name, noc.clock_time);
  result &= getOrFail<double>(config, "wbit_rate", file_name, noc.wbit_rate);
  result &= getOrFail<double>(config, "token_pass_time", file_name, noc.token_pass_time);
  result &= getOrFail<double>(config, "memory_bandwidth", file_name, memory_bandwidth);
  result &= getOrFail<int>(config, "bits_instruction", file_name, bits_instruction);
  result &= getOrFail<double>(config, "decode_time_per_instruction", file_name, decode_time_per_instruction);
  result &= getOrFail<double>(config, "t1", file_name, t1);
  result &= getOrFail<bool>(config, "stats_detailed", file_name, stats_detailed);
  result &= getOrFail<double>(config, "qscale_factor", file_name, qscale_factor);

  // Set seed used for random number generator. If seed==0, it is set
  // to the current time
  result &= getOrFail<unsigned>(config, "seed", file_name, seed);
  if (seed == 0)
    seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  
  return result;
}

void Parameters::updateEPRDelay(const double nv)
{
  epr_delay = nv;
}

void Parameters::updateDistDelay(const double nv)
{
  dist_delay = nv;
}

void Parameters::updatePreDelay(const double nv)
{
  pre_delay = nv;
}

void Parameters::updatePostDelay(const double nv)
{
  post_delay = nv;
}

void Parameters::updateNoCClockTime(const double nv)
{
  noc.clock_time = nv;
}
  
void Parameters::updateWBitRate(const double nv)
{
  noc.wbit_rate = nv;
}

void Parameters::updateTokenPassTime(const double nv)
{
  noc.token_pass_time = nv;
}

void Parameters::updateMemoryBandwidth(const double nv)
{
  memory_bandwidth = nv;
}

void Parameters::updateBitsInstruction(const int nv)
{
  bits_instruction = nv;
}

void Parameters::updateDecodeTime(const double nv)
{
  decode_time_per_instruction = nv;
}

void Parameters::updateThermalRelaxationTime(const double nv)
{
  t1 = nv;
}

void Parameters::updateStatsDetailed(const bool nv)
{
  stats_detailed = nv;
}

void Parameters::updateQScaleFactor(const double nv)
{
  qscale_factor = nv;
}

void Parameters::updateSeed(const unsigned nv)
{
  seed = nv;
}

void Parameters::scaleQuantumRelatedParameters()
{
  for (auto& kv : gate_delays)
    kv.second *= qscale_factor;

  epr_delay  *= qscale_factor;
  dist_delay *= qscale_factor;
  pre_delay  *= qscale_factor;
  post_delay *= qscale_factor;
}
