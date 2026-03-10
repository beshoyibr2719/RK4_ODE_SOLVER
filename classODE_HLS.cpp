#include "ap_int.h"
#include "hls_math.h"

//Owner: Beshoy Ibrahim
// Freshman CE at UIC

// ODE class handles all the math for solving a system of 
// ODEs using RK4
class ODE
{
// our private variables in the class
    private:
    double t_, h_, g_;
    double y_[2];
    int steps_;
// computes the projectile motion ODE: dy/dt = v, 
// dv/dt = -g
    void ode_func(double t, double y[2], double g, double result[2])
    {
        result[0] = y[1];
        result[1]= -g;
    }

// one RK4 step, fills result with the next y values
void rk4_step(double result[2]){
    double z = h_/2.0; //half step
    double x = t_ + z; // midpt time
    double a = h_/6.0; // RK4 scaling factor
// temp holds intermediate y values for k2, k3, k4 calculations
    double temp[2];
// sum holds the final weighted combination of k values
    double sum[2];

// k1 calculation

    double k1[2];
    ode_func(t_, y_, g_, k1);
    for(int i =0; i < 2; i++)
    {
        temp[i] = y_[i] + z * k1[i];
    }
// k2 calculation using midpt
    double k2[2];
    ode_func(x, temp ,g_,k2);

    for(int i =0; i < 2; i++)
    {
        temp[i] = y_[i] + z * k2[i];
    }
// k3 calculation using midpt
    double k3[2];
    ode_func(x, temp, g_, k3);

    for(int i =0; i < 2; i++)
    {
        temp[i] = y_[i] + h_ * k3[i];
    }
// k4 calculation using full step
    double k4[2];
    ode_func(t_ + h_, temp, g_, k4);
// sum up fweighted k values for this step
    for(int i =0; i < 2; i++)
    {
        sum[i] = y_[i] + a*k1[i] + 2*a*k2[i] + 2*a*k3[i] + a*k4[i];
    }
// copy the result out
    for(int i =0; i < 2; i++)
    {
        result[i] = sum[i];
    }
}

    public:
// add the constructor
// constructor: sets up starting values for the solver
    ODE(double t, double y[2], double h, int steps, double g):t_(t), h_(h), steps_(steps), g_(g){
        for(int i =0; i < 2; i++)
        {
            y_[i] = y[i];
        }
    }
// loops through all steps and updates y and t each iteration
void solve(){
double result[2];
    for(int i = 0; i < steps_; i++)
    {
        rk4_step(result);
    // copy result back into y_ for next step
        for(int i = 0; i < 2; i++)
        {
            y_[i] = result[i];
        }
        t_ += h_;
    }
}
};

// top level HLS entry pt, inputs become hardware ports

void hls_ode(double t0, double h, double g, int steps, double x0, double v0)
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=t0
#pragma HLS INTERFACE s_axilite port=h
#pragma HLS INTERFACE s_axilite port=x0
#pragma HLS INTERFACE s_axilite port=steps
#pragma HLS INTERFACE s_axilite port=v0
#pragma HLS INTERFACE s_axilite port=g

// pack initial conditions into array and run the solver
    double y0[2] = {x0, v0};
    ODE solver(t0, y0, h, steps, g);
    solver.solve();
}

