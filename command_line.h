// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: command_line.h
// Description: Declaration of command_line related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__

#include <map>
#include "parameters.h"
#include "architecture.h"

bool checkCommandLine(int argc, char* argv[],
		      string& circuitfn, string& architecturefn, string& parametersfn,
		      map<string,string>& params_override);

void overrideParameters(const map<string,string>& params_override,
			Architecture& arch, Parameters& params);
#endif
