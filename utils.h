// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: utils.h
// Description: Declaration of utilities functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <yaml-cpp/yaml.h>

#define ERR_FATAL     1
#define ERR_ARCH_FILE 2  // exit error code if error while reading architecture file
#define ERR_CIRC_FILE 3  // exit error code if error while reading circuit file
#define ERR_PARM_FILE 4  // exit error code if error while reading parameters file
#define ERR_UNDEF_GATE_DELAY 5 // exit error code if gate delay not found

#define IND "  " // Indentation string used in YAML generated files

#define FATAL(msg) fatalError(msg, __FILE__, __func__, __LINE__)

using namespace std;

/**
 * @brief Reports a fatal error with contextual information and exits the program.
 *
 * This function prints a custom error message along with the file name,
 * function name, and line number where the error occurred. It then terminates
 * the program using exit(1), indicating an abnormal termination to the OS.
 *
 * @param message Custom error message describing the failure
 * @param file    The name of the source file (__FILE__)
 * @param func    The name of the function (__func__)
 * @param line    The line number in the source file (__LINE__)
 */
void fatalError(const string& message, const char* file, const char* func, int line);

// Show the banner
void showBanner();

// Show the commandline
void showCommandLine(int argc, char* argv[]);

// Start chrono
void startChrono(chrono::high_resolution_clock::time_point& chrono_start);

// Stop chrono and returns elapsed time in seconds from chrono_start
double stopChrono(const chrono::high_resolution_clock::time_point& chrono_start);

// Returns a string with the current date and time in "Y-m-d H:M:S" format
string getCurrentDateTimeString();

// Loads a YAML file. Returns true if successful, false otherwise, and
// prints a specific error message to stderr
bool loadYAMLFile(const string& file_name, YAML::Node& config);

// reads the value associated to a key of a yaml node and print a
// message on stderr in case of error. This function is used by
// readFromFile method of Architecture and Parameters
template <typename T>
bool getOrFail(const YAML::Node& node, const string& key, const string& file_name, T& val)
{
  try {
    val = node[key].as<T>();
    return true;
  } catch (const YAML::BadConversion& e) {
    cerr << "Error: '" << key << "' not defined or has wrong type in " << file_name << endl;
    return false;
  } catch (const YAML::InvalidNode& e) {
    cerr << "Error: '" << key << "' missing in " << file_name << endl;
    return false;
   }
}

// returns a random integer between 0 and prob.size()-1 with prob(i) =
// prob[i]
int getRandomNumber(const vector<float> prob);

// returns a set of size set_size of random integer without repetition
// between 0 and n-1
set<int> getRandomNoRepetition(const int n, const int set_size);

// Coherency is computed based on P. Escofet, et al. 2025. An Accurate
// and Efficient Analytic Model of Fidelity Under Depolarizing Noise
// Oriented to Large Scale Quantum System Design. arXiv:2503.06693
// [quant-ph] https://arxiv.org/abs/2503.06693
// Times are in seconds
double computeCoherence(const double etime, const double t1);

#endif
