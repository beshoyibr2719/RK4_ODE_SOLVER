#include <iostream> 
#include "ODE_HLS.h"
int main()
{
    double t = 0.0;
    double y = 0.5;
    double h = 0.1;
    int steps = 20;

    for(int i = 0; i < steps; i++)
    {
        std::cout << "t = " << t << ", y = " << y << std::endl;
        y = rk4_step(t, y, h);
        t += h;
    }
    return 0;
}