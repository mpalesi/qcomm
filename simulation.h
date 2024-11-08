#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include "architecture.h"
#include "core.h"
#include "circuit.h"
#include "mapping.h"
#include "statistics.h"
#include "noc.h"
#include "parameters.h"

struct Simulation
{
  bool isLocalGate(const Gate& gate, const Mapping& mapping);
  void splitLocalRemoteGates(const ParallelGates& pgates, const Mapping& mapping,
			     ParallelGates& lgates, ParallelGates& rgates);
  Statistics localExecution(const ParallelGates& lgates,
			    const Parameters& params);
  int selectDestinationCore(const Gate& gate, const Mapping& mapping, const Cores& cores);
  void updateMappingAndCores(const Architecture& architecture,
			     Mapping& mapping, Cores& cores,
			     const Gate& gate, const int dst_core);
  void addParallelCommunications(ParallelCommunications& parallel_communications,
				 const Gate& gate, const int dst_core,
				 const Mapping& mapping, const int volume);
  
  CommunicationTime getCommunicationTime(const ParallelCommunications& pcomms,
					 const NoC& noc,
					 const Parameters& params);
  void addCommunicationTime(CommunicationTime& total_ct,
			    const CommunicationTime& ct);
  void updateRemoteExecutionStats(Statistics& stats,
				  const ParallelGates& pgates,
				  const ParallelCommunications& pcomms,
				  const NoC& noc,
				  const Parameters& params);
  void removeExecutedGates(const ParallelGates& scheduled_gates,
			   ParallelGates& gates);

  Statistics remoteExecution(const Architecture& architecture, const NoC& noc,
			     const Parameters& parameters,
			     const ParallelGates& rgates,
			     Mapping& mapping, Cores& cores);
  Statistics mergeLocalRemoteStatistics(const Statistics& stats_local,
					const Statistics& stats_remote);

  void fetchContribution(Statistics& stats,
			 const ParallelGates& pgates,
			 const Architecture& architecture,
			 const Parameters& parameters);
  void decodeContribution(Statistics& stats,
			  const ParallelGates& pgates,
			  const Parameters& parameters);
  void dispatchContribution(Statistics& stats,
			    const ParallelGates& pgates,
			    const Architecture& architecture,
			    const Parameters& parameters,
			    const Mapping& mapping,
			    const NoC& noc);
  ParallelCommunications makeDispatchCommunications(const ParallelGates& pgates,
						    const Architecture& architecture,
						    const Parameters& parameters,
						    const Mapping& mapping);

  Statistics simulate(const ParallelGates& pgates, const Architecture& architecture,
		      const NoC& noc, const Parameters& parameters,
		      Mapping& mapping, Cores& cores);
  Statistics simulate(const Circuit& circuit, const Architecture& architecture,
		      const NoC& noc, const Parameters& parameters,
		      Mapping& mapping, Cores& cores);

};

#endif
