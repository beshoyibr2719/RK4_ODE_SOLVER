// for HLS later in Vitis! #include "ap_int.h"
#include <math.h>
#include "ODE_HLS.h"
double f(double t, double y)
{
// tells HLS to copy this function's logic directly into the calling function
// making it so we do not have to create a seperate hardware block every time!
    #pragma HLS INLINE
    return y - t*t + 1;
}
double rk4_step(double t, double y, double h)
    {
// allows us to work on multiple calls of rk4_step simultaneously.
    #pragma HLS PIPELINE

        
    double z = h/2.0;
    double x = t + z;
    double k1 = f(t, y);
    double k2 = f(x, y + z * k1);
    double k3 = f(x, y + z * k2);
    double k4 = f(t + h, y + h * k3);
    double a = h/6.0;
    double sum = y + (a*k1);
    sum += (a*2) * k2;
    sum += (a*2) * k3;
    sum += a * k4;
    return sum;
 }
