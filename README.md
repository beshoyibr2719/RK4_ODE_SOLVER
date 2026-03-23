I am continuing to build this project as part of my freshman year 
research work, where I am exploring how numerical methods can be 
accelerated on FPGAs using High Level Synthesis (HLS).

## What it does?
This is a C++ implementation of the Runge-Kutta 4th Order (RK4) method 
for solving systems of ordinary differential equations (ODEs). Instead 
of a basic procedural approach, I designed it using OOP principles so 
it is able to map cleanly onto FPGA hardware through HLS tools like 
Vitis HLS.

## What I have learned while building this!
- How to design flexible C++ classes with private state and public methods
- Using lambda functions to pass any ODE equation into the solver
- Extending scalar solvers to handle full systems of ODEs with vector state
- Beginning to understand how clean OOP structure helps HLS compilers 
  pipeline and parallelize computation
- How to package HLS IP and integrate it into a Vivado block design
- How the AXI-Lite interface works as a register map between the ARM 
  processor and custom FPGA hardware
- How 64-bit doubles get split into two 32-bit registers across the 
  AXI-Lite bus
- Practical lessons about toolchain friction: OneDrive conflicts with 
  Vivado, XML file handling, and IP catalog compatibility settings

## Example
Tested with projectile motion under gravity where position and velocity 
are solved simultaneously. Output matched expected physics which 
validated the implementation.

Inputs: t0=0.0, h=0.1, g=9.8, steps=10, x0=0.0, v0=10.0
Output: x=5.1000, v=0.2000 ✓ verified on hardware

## Why OOP for FPGA?
Encapsulation gives HLS tools clear boundaries to optimize. Private 
variables become registers, methods become pipelines, and the clean 
structure maximizes parallelization on hardware.

## Project Status (ODE SYSTEM)
- [x] C simulation passing
- [x] C synthesis passing
- [x] IP packaged and ready for Vivado
- [x] Vivado block design complete
- [x] Bitstream generated and deployed to PYNQ-Z2
- [x] AXI-Lite register communication working from Jupyter notebook
- [x] Hardware output verified: x=5.1, v=0.2 matches software result
- [ ] Performance benchmarking vs CPU implementation
- [ ] Scaling to larger ODE systems
- [ ] Timing closure (currently -0.09ns slack)

## HLS Optimization Trials
| Trial | Pragmas | DSP | FF | LUT | Timing Met? |
|-------|---------|-----|----|-----|-------------|
| Baseline | None | 23 | 4,555 | 5,172 | No (-0.09ns) |
| Pipeline only | HLS PIPELINE in solve() | 56 | 5,088 | 5,813 | No (-0.09ns) |
| Unroll only | HLS UNROLL in k1 loop | 23 | 4,555 | 5,172 | No (-0.09ns) |
| Both | PIPELINE + UNROLL | 56 | 5,088 | 5,813 | No (-0.09ns) |

KEY FINDINGS: For a simple 2-equation system, HLS already 
auto-optimizes small loops. PIPELINE increased resource usage without 
timing improvement. Pragmas will have more impact as the ODE system 
scales and becomes more complex.

## Deployment
The IP is deployed on a PYNQ-Z2 board and controlled via a Jupyter 
notebook using the PYNQ Python library. The Zynq ARM processor writes 
input values to AXI-Lite registers, triggers execution, and reads back 
results from the hardware accelerator.

Files needed to run on PYNQ:
- `rk4_ode.bit` — bitstream file
- `rk4_ode.hwh` — hardware handoff file
- `ODE Accelerator.ipynb` — Jupyter notebook

## Repository Structure
- `classODE.cpp` — Full featured OOP RK4 solver (software version)
- `ODE_HLS.cpp` — HLS compatible version for FPGA deployment
- `classODE_HLS.cpp` — HLS compatible version for FPGA deployment 
   for a system of ODEs
- `classODE_HLStb.cpp` — testbench for system of ODEs version
- `classODE_HLS.h` — header file for HLS version
- `ODE Accelerator.ipynb` — PYNQ Jupyter notebook for hardware control
- `README.md` — Project documentation
