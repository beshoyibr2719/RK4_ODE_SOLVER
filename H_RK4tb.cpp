#include <iostream> 
#include "H_RK4.h"
int main()
{
    double t0 = 0.0;
    double dt = 0.01;
    int steps = 1000;
    double V0 = 0.0;
    double n0 = 0.317;
    double m0 = 0.05;
    double h0 = 0.6;
    double out_V;
    double out_n;
    double out_m;
    double out_h;

    hls_ode(t0, dt, steps, V0, n0, m0, h0, &out_V, &out_n, &out_m, &out_h);
    std::cout << "simulation complete" << std::endl;
    std::cout << "Final voltage V = " << out_V << ", Final potassium n = " << out_n << ", Final sodium activation m = " << out_m << ", Final sodium inactivation h = " << out_h << std::endl;
    return 0;
}