I am contiuning to build this project as part of my freshman year research work, where I am exploring how numerical methods can be accelerated on FPGAs using High Level Synthesis (HLS).

What it does?
This is a C++ implementation of the Runge-Kutta 4th Order (RK4) method for solving systems of ordinary differential equations (ODEs). Instead of a basic procedural approach, I designed it using OOP principles so it is able to map cleanly onto FPGA hardware through HLS tools like Vitis HLS in the future.

What I have learned while building this!
How to design flexible C++ classes with private state and public methods
Using lambda functions to pass any ODE equation into the solver
Extending scalar solvers to handle full systems of ODEs with vector state
Beginning to understand how clean OOP structure helps HLS compilers pipeline and parallelize computation
Example
Tested with projectile motion under gravity where position and velocity are solved simultaneously. Output matched expected physics which validated the implementation.

Why OOP for FPGA?
Encapsulation gives HLS tools clear boundaries to optimize. Private variables become registers, methods become pipelines, and the clean structure maximizes parallelization on hardware.

## Project Status(ODE SYSTEM)
- [x] C simulation passing
- [x] C syntheis passing
- [x] IP packaged and ready for Vivado
- [ ] Vivado block design- pedning PYNQ Z2 board files

| Trial | Pragmas | DSP | FF | LUT | Timing Met? |
|-------|---------|-----|----|-----|-------------|

| Baseline | None | 23 | 4,555 | 5,172 | No (-0.09ns) |

| Pipeline only | HLS PIPELINE in solve() | 56 | 5,088 | 5,813 | No (-0.09ns) |

| Unroll only | HLS UNROLL in k1 loop | 23 | 4,555 | 5,172 | No (-0.09ns) |

| Both | PIPELINE + UNROLL | 56 | 5,088 | 5,813 | No (-0.09ns) |

KEY FINDINGS: For a simple 2-equation system, HLS already auto-optimizes small loops! PIPELINE increased resource usage without timing improvement. Pragmas will have more impact as the ODE system scales and becomes more complex

## Repository Structure
- `classODE.cpp` — Full featured OOP RK4 solver (software version)
- `ODE_HLS.cpp` — HLS compatible version for FPGA deployment
- `README.md` — Project documentation
- `classODE_HLS.cpp` — HLS compatible version for FPGA deployment for a system of ODEs (in progress)
- `classODE_HLStb.cpp` — test bench for system of ODEs version
- `classODE_HLS.h` - header file for HLS version
