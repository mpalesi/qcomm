#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <set>

#define ERR_ARCH_FILE 1  // exit error code if error while reading architecture file
#define ERR_CIRC_FILE 2  // exit error code if error while reading circuit file
#define ERR_PARM_FILE 3  // exit error code if error while reading parameters file

#define IND "  " // Indentation string used in YAML generated files

using namespace std;

// returns a random integer between 0 and prob.size()-1 with prob(i) =
// prob[i]
int getRandomNumber(const vector<float> prob);

// returns a set of size set_size of random integer without repetition
// between 0 and n-1
set<int> getRandomNoRepetition(const int n, const int set_size);

// compute the coherence
double computeCoherence(const double etime, const double t1);

#endif
