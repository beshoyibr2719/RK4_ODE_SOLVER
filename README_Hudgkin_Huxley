## Hodgkin-Huxley Neuron Model on FPGA

This is the next phase of my FPGA research, extending my RK4 ODE solver 
to simulate a single neuron using the Hodgkin-Huxley model. The same 
OOP class structure and RK4 method from the projectile motion solver 
maps directly to this — the only thing that changes is the math inside 
the ODE function and the number of state variables.

## What is the Hodgkin-Huxley Model?

The Hodgkin-Huxley model describes how neurons fire by modeling the 
flow of ions across a cell membrane. It was developed in 1952 and is 
still the foundation of computational neuroscience today. The model 
captures how sodium and potassium ion channels open and close in 
response to voltage changes, producing the characteristic spike shape 
of a neuron firing action potential.

## The 4 State Variables

| Variable | Meaning |
|----------|---------|
| V | Membrane voltage — the most important state, drives everything else |
| n | Potassium channel gating variable |
| m | Sodium activation gating variable |
| h | Sodium inactivation gating variable |

Each variable has its own ODE describing how it changes over time, 
making this a system of 4 coupled equations — compared to the 2 
equations in the projectile motion solver.

## Why RK4 Maps Directly to Hodgkin-Huxley

The RK4 solver I built for projectile motion was designed from the 
start to handle any system of ODEs through OOP. The state vector y[] 
just grows from size 2 to size 4. The RK4 algorithm itself does not 
change at all — only the derivative function changes to implement the 
H-H equations instead of simple projectile physics.
```
Projectile:  y[0]=x, y[1]=v         (2 equations)
H-H Neuron:  y[0]=V, y[1]=n, y[2]=m, y[3]=h   (4 equations)
```

## The Math
```
Cm * dV/dt = gNa*m³h(ENa-V) + gK*n⁴(EK-V) + gL(EL-V)
dm/dt = αm(V)(1-m) - βm(V)m
dh/dt = αh(V)(1-h) - βh(V)h  
dn/dt = αn(V)(1-n) - βn(V)n
```

Constants used: Cm=1.0, gNa=120.0, gK=36.0, gL=0.3, 
ENa=115.0, EK=-12.0, EL=10.6

## C Simulation Results

Initial conditions: t0=0.0, dt=0.01, steps=1000, V0=0.0, 
n0=0.317, m0=0.05, h0=0.6

Output: V=2.53998, n=0.304944, m=0.0692, h=0.699746 ✓ C simulation passing

All 4 state variables evolved correctly over time confirming the 
H-H equations are implemented correctly.

## HLS Optimization Trials

| Trial | Pragmas | DSP | FF | LUT | Timing Met? |
|-------|---------|-----|----|-----|-------------|
| Baseline | None | 561 | 42,779 | 67,053 | No (-1.16ns) |
| Pipeline in solve() | HLS PIPELINE | 561 | 42,906 | 66,869 | No (-1.16ns) |
| Unroll in k loops | HLS UNROLL | 279 | 21,991 | 34,716 | No (-1.31ns) |
| Pipeline on ode_func | HLS PIPELINE | 561 | 42,779 | 67,053 | No (-1.16ns) |
| Both | UNROLL + PIPELINE | 561 | 42,617 | 66,961 | No (-1.16ns) |

KEY FINDINGS: UNROLL on the k loops in rk4_step is the best pragma 
combination — cuts DSP usage in half and reduces FF and LUT by almost 
half. PIPELINE had no effect anywhere because the bottleneck is the 
sequential exponential math in ode_func which has no loops to pipeline. 
Combining both pragmas negated the resource savings from unroll alone 
as pipeline duplication fought against unroll's efficiency gains.

This is a significant contrast to the 2-equation projectile solver 
where pragmas made no difference at all — the added complexity of H-H 
gives HLS more parallelism to exploit.

## Project Status

- [x] C simulation passing
- [x] C synthesis complete — optimization trials done
- [x] Best pragma combination identified — UNROLL on k loops
- [ ] IP packaging
- [ ] Vivado block design
- [ ] Bitstream generated and deployed to PYNQ-Z2
- [ ] Hardware output verified
- [ ] Performance benchmarking vs CPU
- [ ] Timing closure (currently -1.16ns slack)

## Repository Structure

- `H_RK4.cpp` — HLS-compatible Hodgkin-Huxley ODE solver
- `H_RK4tb.cpp` — testbench with standard H-H initial conditions
- `H_RK4.h` — header file for HLS top function
