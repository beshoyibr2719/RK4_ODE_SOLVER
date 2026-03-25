// needed for HLS!!
// arbitrary percision
#include "ap_int.h"
// allows us to use hardware friendly math functions
#include "hls_math.h"

//Owner: Beshoy Ibrahim
// Freshman CE at UIC

// ODE class handles all the math for solving a system of ODEs using RK4 method
class ODE
{
// our private variables in the class
    private:
    double dt_;
    double t_;
    double y_[4];
    int steps_;
// Solves for simulation of one neuron using Hodgkin Huxley Model for the math: 
// dVm/dt = how voltage changes over time
// dn/dt = potassium channel gating variable
// dm/dt = sodium activation gating variable
// dh/dt = sodium inactivatioin gating variable
// y[2] -> position and velocity

// constants needed for math: 
// - `Cm = 1.0`, `gNa = 120.0`, `gK = 36.0`, `gL = 0.3`
// - `ENa = 115.0`, `EK = -12.0`, `EL = 10.6`
    void ode_func(double t, double y[4],  double result[4])
    {
        #pragma HLS PIPELINE
        double V = y[0];
        double n = y[1];
        double m = y[2];
        double h = y[3];

        result[0] = (1.0/1.0) * 120.0*(m*m*m)*h*(115-V) + 36 * (n*n*n*n)*(-12.0 - V) + 0.3 * (10.6 - V);
        double alpha_n = 0.01 * (V+10) / (hls::exp((V+10)/10.0) - 1);
        double beta_n  = 0.125 * hls::exp(V/80.0);
        result[1] = alpha_n*(1 - n) - beta_n*n;

        double alpha_m = 0.1 * (25-V) / (hls::exp((25-V)/10.0) - 1);
        double beta_m  = 4.0 * hls::exp(-V/18.0);
        result[2] = alpha_m*(1 - m) - beta_m*m;


        double alpha_h = 0.07  * hls::exp(-V/20);
        double beta_h  = 1.0 / hls::exp((30 - V)/10 + 1);
        result[3] = alpha_h*(1 - h) - beta_h*h;

    }

// one RK4 step, fills result with the next y values
void rk4_step(double result[4]){
    double z = dt_/2.0; //half step
    double x = t_ + z; // midpt time
    double a = dt_/6.0; // RK4 scaling factor
// temp holds intermediate y values for k2, k3, k4 calculations
    double temp[4];
// sum holds the final weighted combination of k values
    double sum[4];

// k1 calculation

    double k1[4];
    ode_func(t_, y_, k1);
    for(int i = 0; i < 4; i++)
    {
// TEST: HLS UNROLL
        #pragma HLS UNROLL
        temp[i] = y_[i] + z * k1[i];
    }
// k2 calculation using midpt
    double k2[4];
    ode_func(x, temp , k2);

    for(int i =0; i < 4; i++)
    {
        #pragma HLS UNROLL
        temp[i] = y_[i] + z * k2[i];
    }
// k3 calculation using midpt
    double k3[4];
    ode_func(x, temp, k3);

    for(int i =0; i < 4; i++)
    {
        #pragma HLS UNROLL
        temp[i] = y_[i] + dt_ * k3[i];
    }
// k4 calculation using full step
    double k4[4];
    ode_func(t_ + dt_, temp, k4);
// sum up fweighted k values for this step
    for(int i =0; i < 4; i++)
    {
        #pragma HLS UNROLL
        sum[i] = y_[i] + a*k1[i] + 2*a*k2[i] + 2*a*k3[i] + a*k4[i];
    }
// copy the result out
    for(int i =0; i < 4; i++)
    {
        #pragma HLS UNROLL
        result[i] = sum[i];
    }
}

    public:

// add the constructor
// constructor: sets up starting values for the solver
    ODE(double t, double y[4], double dt, int steps):t_(t), dt_(dt), steps_(steps)
    {
        for(int i =0; i < 4; i++)
        {
            y_[i] = y[i];
        }
    }

// getter methods to get voltage(V)

    double get_V()
    {
        return y_[0];
    }
    double get_n()
    {
        return y_[1];
    }
    double get_m()
    {
        return y_[2];
    }
    double get_h()
    {
        return y_[3];
    }


// loops through all steps and updates y and t each iteration
void solve(){
double result[4];
    for(int i = 0; i < steps_; i++)
    {
// TEST: trying pipeline pragma because this loop runs a lot!
        rk4_step(result);
    // copy result back into y_ for next step
        for(int i = 0; i < 4; i++)
        {
            y_[i] = result[i];
        }
        t_ += dt_;
    }
}
};

// top level HLS entry pt, inputs become hardware ports
// in HLS output parameters are passed as pointers!
void hls_ode(double t0, double dt, int steps, double v0 , double n0, double m0, double h0, double *out_v, double *out_n, double *out_m, double *out_h)
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=t0
#pragma HLS INTERFACE s_axilite port=dt
#pragma HLS INTERFACE s_axilite port=v0
#pragma HLS INTERFACE s_axilite port=steps
#pragma HLS INTERFACE s_axilite port=n0
#pragma HLS INTERFACE s_axilite port=m0
#pragma HLS INTERFACE s_axilite port=h0
#pragma HLS INTERFACE s_axilite port=*out_v
#pragma HLS INTERFACE s_axilite port=*out_n
#pragma HLS INTERFACE s_axilite port=*out_m
#pragma HLS INTERFACE s_axilite port=*out_h

// pack initial conditions into array and run the solver
    double y0[4] = {v0, n0, m0, h0};
    ODE solver(t0, y0, dt, steps);
    solver.solve();
// assign our pointers to results so we can print!
    *out_v = solver.get_V();
    *out_n = solver.get_n();
    *out_m = solver.get_m();
    *out_h = solver.get_h();
}

