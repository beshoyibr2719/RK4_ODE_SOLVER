#ifndef ODE_HLS_H
#define ODE_HLS_H

void hls_ode(double t0, double dt, int steps, double v0, double n0, double m0, double h0, double *out_v, double *out_n, double *out_m, double *out_h);

#endif