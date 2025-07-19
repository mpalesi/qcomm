#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <vector>
#include <set>
#include <yaml-cpp/yaml.h>

#define ERR_ARCH_FILE 1  // exit error code if error while reading architecture file
#define ERR_CIRC_FILE 2  // exit error code if error while reading circuit file
#define ERR_PARM_FILE 3  // exit error code if error while reading parameters file

#define IND "  " // Indentation string used in YAML generated files

using namespace std;

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

// compute the coherence
double computeCoherence(const double etime, const double t1);

#endif
