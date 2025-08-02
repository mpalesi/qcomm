// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: utils.cpp
// Description: Implementation of utilities functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <random>
#include "utils.h"

//----------------------------------------------------------------------
void showBanner()
{
  cout << R"(
#   __ _  ___ ___  _ __ ___  _ __ ___  
#  / _` |/ __/ _ \| '_ ` _ \| '_ ` _ \ 
# | (_| | (_| (_) | | | | | | | | | | |
#  \__, |\___\___/|_| |_| |_|_| |_| |_|
#    |_|                              
#        
#        qcomm - Quantum Communication Simulator
#        Author: Maurizio Palesi (University of Catania)
#        Email: maurizio.palesi@unict.it
#        GitHub: https://github.com/mpalesi/qcomm
)" << endl;
}

//----------------------------------------------------------------------
void fatalError(const string& message, const char* file, const char* func, int line)
{
  cerr << "Fatal error: " << message << endl;
  cerr << "Location: " << file << ":" << line << " in " << func << "()" << endl;
  exit(ERR_FATAL);  // Graceful termination with error code
}

//----------------------------------------------------------------------
void showCommandLine(int argc, char* argv[])
{
  cout << "command_line: '";
  for (int i=0; i<argc; i++) {
    cout << argv[i];
    if (i != argc-1)
      cout << " ";
  }
  
  cout << "'" << endl;
}

//----------------------------------------------------------------------
void startChrono(chrono::high_resolution_clock::time_point& chrono_start)
{
  chrono_start = chrono::high_resolution_clock::now();
}

double stopChrono(const chrono::high_resolution_clock::time_point& chrono_start)
{
  auto chrono_end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = chrono_end - chrono_start;
  return elapsed.count(); // seconds
}

//----------------------------------------------------------------------
string getCurrentDateTimeString()
{
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now_time));
    return string(buffer);
}

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
