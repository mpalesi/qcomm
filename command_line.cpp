#include <iostream>
#include "command_line.h"

using namespace std;

bool checkCommandLine(int argc, char* argv[],
		      string& circuitfn, string& architecturefn, string& parametersfn,
		      map<string,string>& params_override)
{
  if (argc < 7)
    return false;

  params_override.clear();
  
  for (int i=1; i<argc; i++)
    {
      string arg = argv[i];
      if (arg == "-a")
	architecturefn = string(argv[++i]);
      else if (arg == "-c")
	circuitfn = string(argv[++i]);
      else if (arg == "-p")
	parametersfn = string(argv[++i]);
      else if (arg == "-o")
	{
	  params_override[string(argv[i+1])] = string(argv[i+2]);
	  i += 2;
	}
      else
	return false;
    }  

  return true;
}

// ----------------------------------------------------------------------
void overrideParameters(const map<string,string>& params_override,
			Architecture& arch, Parameters& params)
{
  for (auto it = params_override.begin(); it != params_override.end(); ++it)
    {
      string param = it->first;
      string value = it->second;

      if (param == "mesh_x")
	arch.updateMeshX(stoi(value));
      else if (param == "mesh_y")	
	arch.updateMeshY(stoi(value));
      else if (param == "link_width")	
	arch.updateLinkWidth(stoi(value));
      else if (param == "qubits_per_core")
	arch.updateQubitsPerCore(stoi(value));
      else if (param == "ltm_ports")
	arch.updateLTMPorts(stoi(value));
      else if (param == "radio_channels")
	arch.updateRadioChannels(stoi(value));
      else if (param == "wireless_enabled")
	arch.updateWirelessEnabled(stoi(value));
      else if (param == "hop_delay")
	params.updateHopDelay(stod(value));
      else if (param == "epr_delay")
	params.updateEPRDelay(stod(value));
      else if (param == "pre_delay")
	params.updatePreDelay(stod(value));
      else if (param == "post_delay")
	params.updatePostDelay(stod(value));
      else if (param == "wbit_rate")
	params.updateWBitRate(stod(value));
      else if (param == "token_pass_time")
	params.updateTokenPassTime(stod(value));
      else
	cout << ">>> Unrecognized parameter '" << param << "' is ignored!" << endl;
    }
}