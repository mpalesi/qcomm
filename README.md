# Qcomm

## About
Welcome to Qcomm, a simulation tool designed to assess the impact
of communication on the performance of scalable multi-core
quantum architectures. Qcomm allows users to adjust various
architectural and physical parameters of the system, and outputs
various communication metrics.

## Publications
We introduced the first version of Qcomm and used it to conduct a preliminary study on how various parameters influence communication performance in a multi-core quantum system. The results of this study were presented in the following publication:

Maurizio Palesi, Enrico Russo, Davide Patti, Giuseppe Ascia, and Vincenzo Catania, "_Assessing the Role of Communication in Scalable Multi-Core Quantum Architectures_," in _2024 IEEE 17th International Symposium on Embedded Multicore/Many-core Systems-on-Chip (MCSoC)_, Kuala Lumpur, Malaysia, 2024, pp. 482-489, doi: 10.1109/MCSoC64144.2024.00085.

If you use Qcomm in your research, we kindly ask you to cite the above publication in any related work.

## Installation and Quick Start

### Dependency

This project uses the [`yaml-cpp`](https://github.com/jbeder/yaml-cpp) library to read YAML configuration files. You must install `yaml-cpp` before compiling the project. Follow the instructions for your operating system:

<details>
<summary><strong>macOS (Homebrew)</strong></summary>

```bash
brew install yaml-cpp
```
After installation, update the `Makefile` with the correct installation path:
```make
YAML_CPP_PREFIX := /opt/homebrew/opt/yaml-cpp
```
</details>

<details> 
<summary><strong>Ubuntu / Debian</strong></summary>
  
```bash
sudo apt-get update
sudo apt-get install libyaml-cpp-dev
```
In most cases, no changes to the `Makefile` are needed. If required, set:
```make
YAML_CPP_PREFIX := /usr
```
</details> 

<details> 
<summary><strong>Windows (vcpkg)</strong></summary>

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # or bootstrap-vcpkg.bat on Windows
./vcpkg install yaml-cpp
```
Then configure your compiler and linker to use the installed package.
</details>

### Compilation Instructions
To compile the project, simply run:
```bash
make
```
This will generate two executable files in the root directory:
* `qcomm`: the actual simulator
*  `rcg`: the random circuit generator

**Note:** Ensure that all dependencies (e.g., `yaml-cpp`) are correctly installed and the `Makefile` is properly configured. In particular, edit the `YAML_CPP_PREFIX` variable in the `Makefile` to point to your system's `yaml-cpp` installation path. See the [Dependency](#dependency) section for installation details.

### How to use `qcomm`
`qcomm` requires three input files:
* a **circuit file**
* an **architecture file** (YAML format)
* a **parameters file** (YAML format)
```bash
./qcomm -c <circuit> -a <architecture> -p <parameters> [-o <parameter> <value> ...]
```
Sample input files can be found in the `samples/` directory.
Run the simulator with:
```bash
./qcomm -c samples/circuit -a samples/architecture.yaml -p samples/parameters.yaml
```
The simulation results are printed to standard output in **YAML format**.

The `-o` command-line option can be used to override the value of any parameter defined in the architecture or parameters file.
For example, to override the value of `noc_clock_time` specified in the parameters file and set it to 0.1e-9, run:
```bash
./qcomm -c samples/circuit -a samples/architecture.yaml -p samples/parameters.yaml -o noc_clock_time 0.1e-9
```

### How to use `rcg`
`rcg` is a command-line tool for generating random quantum circuits.
```bash
./rcg <nqubits> <ngates> <prob1 prob2 ... prob_n>
```
Here:
* `<nqubits>` is the number of qubits in the circuit
* `<ngates>` is the number of gates in the circuit
* `prob_i` is the probability to instantiating a gate with *i* inputs.

The generated circuit is printed to standard output.
