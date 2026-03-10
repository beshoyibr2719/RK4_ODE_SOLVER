#include <iostream> 
#include "classODE_HLS.h"
int main()
{
    double t = 0.0;
    double h = 0.1;
    int steps = 10;
    double x0 = 0.0;
    double v0 = 10.0;
    double g = 9.8;

    hls_ode(t, h, g, steps, x0, v0);
    std::cout << "simulation complete" << std::endl;
    return 0;
}