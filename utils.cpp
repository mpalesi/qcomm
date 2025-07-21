// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: utils.cpp
// Description: Implementation of utilities functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <random>
#include "utils.h"

//----------------------------------------------------------------------
bool loadYAMLFile(const string& file_name, YAML::Node& config)
{
  try {
    config = YAML::LoadFile(file_name);
    return true;
  } catch (const YAML::BadFile& e) {
    std::cerr << "Error: cannot open file '" << file_name << "'" << std::endl;
    return false;
  } catch (const YAML::ParserException& e) {
    std::cerr << "Error: invalid YAML syntax in '" << file_name << "': " << e.what() << std::endl;
    return false;
  }
}

//----------------------------------------------------------------------
int getRandomNumber(const vector<float> prob)
{
  random_device rd;
  mt19937 gen(rd());

  std::discrete_distribution<int> dist(prob.begin(), prob.end());

  return dist(gen);
}

//----------------------------------------------------------------------
set<int> getRandomNoRepetition(const int n, const int set_size)
{
  vector<int> numbers;

  for (int i = 0; i < n; ++i)
    numbers.push_back(i);

  random_device rd;
  mt19937 gen(rd());

  shuffle(numbers.begin(), numbers.end(), gen);

  set<int> result;
  for (int i = 0; i < set_size; ++i)
    result.insert(numbers[i]);

  return result;
}

//----------------------------------------------------------------------
double computeCoherence(const double etime, const double t1)
{
  return exp(-etime / t1);
}
