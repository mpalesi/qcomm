""" 
=============================================================================
Project: qcomm - Quantum Communication Simulator
File: qasm2qcomm.py
Description:
qasm2qcomm - QASM to QCOMM Parallel Slicer

This tool parses a QASM 2.0 file using Qiskit, inlines all user-defined gates, 
and emits the quantum circuit as a sequence of parallelizable gate slices. 
Each line of output represents a set of gates that can be executed simultaneously,
as they act on disjoint sets of qubits and respect gate dependencies.

Features:
- Parses OpenQASM 2.0 input using Qiskit
- Inlines composite (user-defined) gates via transpilation
- Excludes measurement operations from the output
- Outputs each gate in the form GATENAME(q0 q1 ...) with qubit indices
- Groups gates into parallel slices while preserving data dependencies

Example usage:
    python qasm2qcomm.py my_circuit.qasm

Output format:
    Each line: GATENAME(q0 q1 ...) GATENAME(q2 ...) ...
    where gates on the same line can be executed in parallel

## Author: Maurizio Palesi <maurizio.palesi@unict.it>
## License: Apache-2.0 license (see LICENSE file for details)
=============================================================================
"""

from qiskit import QuantumCircuit, transpile
import sys

def get_linear_gate_list(qasm_file):
    # Load and inline all gates
    circuit = QuantumCircuit.from_qasm_file(qasm_file)
    basis_gates = ['u3', 'cx', 'x', 'y', 'z', 'h', 's', 'sdg', 't', 'tdg']
    circuit = transpile(circuit, basis_gates=basis_gates, optimization_level=0)

    # Map qubits to indices
    qubit_indices = {q: i for i, q in enumerate(circuit.qubits)}

    # Build linear gate list, skipping measurements
    gate_list = []
    for instr in circuit.data:
        if instr.operation.name == "measure":
            continue  # skip measurements
        gate_name = instr.operation.name.upper()
        qubits = [qubit_indices[q] for q in instr.qubits]
        gate_list.append((gate_name, qubits))
    return gate_list

def form_slices(gate_list):
    slices = []
    qubit_ready_time = {}  # qubit index → when it's next free

    for gate_name, qubits in gate_list:
        # Determine the earliest time this gate can be placed
        earliest_time = max([qubit_ready_time.get(q, 0) for q in qubits], default=0)

        # Extend slices list if needed
        while len(slices) <= earliest_time:
            slices.append([])
        
        # Add gate to the slice
        slices[earliest_time].append((gate_name, qubits))

        # Mark qubits as busy until after this time step
        for q in qubits:
            qubit_ready_time[q] = earliest_time + 1

    return slices

def print_slices(slices):
    for slice_group in slices:
        line = []
        for gate_name, qubits in slice_group:
            q_str = ' '.join(str(q) for q in qubits)
            line.append(f"{gate_name}({q_str})")
        print(' '.join(line))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: python {sys.argv[0]} your_file.qasm")
        sys.exit(1)

    qasm_path = sys.argv[1]
    gate_list = get_linear_gate_list(qasm_path)
    slices = form_slices(gate_list)
    print_slices(slices)
