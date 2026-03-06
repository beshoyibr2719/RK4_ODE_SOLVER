#include "ap_fixed.h"
#include <math.h>

double f(double t, double y)
{
    return y - t*t + 1;
}

double rk4_step(double t, double y, double h)
{
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
