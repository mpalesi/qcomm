#include <iostream>
#include <fstream>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include "utils.h"
#include "parameters.h"

void Parameters::display() const
{
  cout << endl
       << "Parameters:" << endl
       << IND << "gate_delay: " << gate_delay << " # sec" << endl
       << IND << "epr_delay: " << epr_delay << " # sec" << endl
       << IND << "dist_delay: " << dist_delay << " # sec" << endl
       << IND << "pre_delay: " << pre_delay << " # sec" << endl
       << IND << "post_delay: " << post_delay << " # sec" << endl
       << IND << "noc_clock_time: " << noc_clock_time << " # sec" << endl
       << IND << "wbit_rate: " << wbit_rate << " # bps" << endl
       << IND << "token_pass_time: " << token_pass_time << " # sec" << endl
       << IND << "memory_mandwidth: " << memory_bandwidth << " # bps" << endl
       << IND << "bits_instruction: " << bits_instruction << " # bits" << endl
       << IND << "decode_time_per_instruction: " << decode_time_per_instruction << " # sec" << endl
       << IND << "t1: " << t1 << " # sec" << endl;
}

bool Parameters::readFromFile(const string& file_name)
{
  YAML::Node config;
  bool result = loadYAMLFile(file_name, config);

  result &= getOrFail<double>(config, "gate_delay", file_name, gate_delay);
  result &= getOrFail<double>(config, "epr_delay", file_name, epr_delay);
  result &= getOrFail<double>(config, "dist_delay", file_name, dist_delay);
  result &= getOrFail<double>(config, "pre_delay", file_name, pre_delay);
  result &= getOrFail<double>(config, "post_delay", file_name, post_delay);
  result &= getOrFail<double>(config, "noc_clock_time", file_name, noc_clock_time);
  result &= getOrFail<double>(config, "wbit_rate", file_name, wbit_rate);
  result &= getOrFail<double>(config, "token_pass_time", file_name, token_pass_time);
  result &= getOrFail<double>(config, "memory_bandwidth", file_name, memory_bandwidth);
  result &= getOrFail<int>(config, "bits_instruction", file_name, bits_instruction);
  result &= getOrFail<double>(config, "decode_time_per_instruction", file_name, decode_time_per_instruction);
  result &= getOrFail<double>(config, "t1", file_name, t1);
  result &= getOrFail<bool>(config, "stats_detailed", file_name, stats_detailed);
  
  return result;
}

void Parameters::updateGateDelay(const double nv)
{
  gate_delay = nv;
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
  noc_clock_time = nv;
}
  
void Parameters::updateWBitRate(const double nv)
{
  wbit_rate = nv;
}

void Parameters::updateTokenPassTime(const double nv)
{
  token_pass_time = nv;
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
