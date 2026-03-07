I built this project as part of my freshman year research work, where I am exploring how numerical methods can be accelerated on FPGAs using High Level Synthesis (HLS).

What it does
This is a C++ implementation of the Runge-Kutta 4th Order (RK4) method for solving systems of ordinary differential equations (ODEs). Instead of a basic procedural approach, I designed it using OOP principles so it is able to map cleanly onto FPGA hardware through HLS tools like Vitis HLS in the future.

What I learned building this
How to design flexible C++ classes with private state and public methods
Using lambda functions to pass any ODE equation into the solver
Extending scalar solvers to handle full systems of ODEs with vector state
Beginning to understand how clean OOP structure helps HLS compilers pipeline and parallelize computation
Example
Tested with projectile motion under gravity where position and velocity are solved simultaneously. Output matched expected physics which validated the implementation.

Why OOP for FPGA?
Encapsulation gives HLS tools clear boundaries to optimize. Private variables become registers, methods become pipelines, and the clean structure maximizes parallelization on hardware.

## Project Status
Currently working on making the RK4 solver fully compatible with 
Vitis HLS for deployment on the PYNQ Z-2 FPGA board. The software 
version is fully functional and verified. The HLS version is in 
active development as part of ongoing freshman research.

## Repository Structure
- `classODE.cpp` — Full featured OOP RK4 solver (software version)
- `ODE_HLS.cpp` — HLS compatible version for FPGA deployment (in progress)
- `README.md` — Project documentation
