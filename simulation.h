// =============================================================================
// Project: qcomm - Quantum Communication Simulator
// File: simulation.h
// Description: Declaration of simulation structures and related functions
// Author: Maurizio Palesi <maurizio.palesi@unict.it>
// License: Apache-2.0 license (see LICENSE file for details)
// =============================================================================

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
  string simulation_date_time;  
  double simulation_runtime;

  void display();
  
  bool isLocalGate(const Gate& gate, const Mapping& mapping);
  void splitLocalRemoteGates(const ParallelGates& pgates, const Mapping& mapping,
			     ParallelGates& lgates, ParallelGates& rgates);
  Statistics localExecution(const ParallelGates& lgates,
			    const Parameters& params);
  int selectDestinationCore(const Architecture& architecture,
			    const Gate& gate, const Mapping& mapping, const Cores& cores);
  void updateMappingAndCores(const Architecture& architecture,
			     Mapping& mapping, Cores& cores,
			     const Gate& gate, const int dst_core);
  void addParallelCommunications(ParallelCommunications& parallel_communications,
				 const Gate& gate, const int dst_core,
				 const Mapping& mapping, const int volume);
  
  TeleportationTime getTeleportationTime(const ParallelCommunications& pcomms,
					 const NoC& noc,
					 const Parameters& params);
  void addTeleportationTime(TeleportationTime& total_ct,
			    const TeleportationTime& ct);
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
			    const Mapping& mapping);
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

  vector<int> computeTPPathMesh(const int qubit_src, const int qubit_dst,
				const Architecture& architecture);
  vector<int> computeTPPath(const int qubit_src, const int qubit_dst,
			    const Architecture& architecture);
  int allocateAncilla(const int core_id,
		      const Architecture& architecture);
  ParallelGates splitRemoteGate(const Gate& gate,
				const Architecture& architecture);
  list<ParallelGates> splitRemoteGates(const ParallelGates& rgates,
				       const Architecture& architecture);
  list<ParallelGates> sequenceParallelGates(const ParallelGates& lgates,
					    const list<ParallelGates>& pgates_list_par);
  ParallelGates insertSequenceParallelGates(list<ParallelGates>::iterator& it_pgates,
					    list<ParallelGates>& circuit,
					    const list<ParallelGates>& pgates_list_seq);
  ParallelGates FixParallelGatesAndUpdateCircuit(list<ParallelGates>::iterator& it_pgates,
						 list<ParallelGates>& circuit,
						 const Architecture& architecture);

  void freeUnusedAncillas(list<ParallelGates>::iterator it_pgates,
			  const list<ParallelGates>& circuit,
			  Mapping& mapping, Cores& cores);
  set<int> getAncillas(const ParallelGates& pg);
  void removeUsedAncillas(set<int>& ancillas, const ParallelGates& pg);
  void freeAncillas(const set<int>& ancilla, Mapping& mapping, Cores& cores);

  double getMaxGateLatency(const ParallelGates& lgates,
			   const map<string,double>& gate_delays);

  ParallelCommunications removeMI2Node0Communications(const ParallelCommunications& pcomms);

};

#endif
