#include <iostream>
#include <math.h>
#include <functional>
#include <vector>
using namespace std;

// create a class
class ODE
{
    // our private variables in the class
    private:
    double t_, h_;
    int steps_;
    // to allow for a system of ODE's y must be a vector 
    // that can expand in number of functions!
    std::vector<double>y_;

    // function to solve for sum for one iteration
    // main does not need to call directly so must be private to ensure no confusion

vector<double> rk4_step(){
    double z = h_/2.0; //half step
    double x = t_ + z; // midpt time
    double a = h_/6.0; // RK4 scaling factor
// vector temp to allow us to change our number of inputs
    vector<double> temp(y_.size());
// vector sum to be able to get our value at te end of each step
    vector<double> sum(y_.size());

    // k1 calculation

    vector<double> k1 = f_(t_,y_);
// must loop through to see what is the size to see how many iterations we need!
    for(int i =0; i < y_.size(); i++)
    {
        temp[i] = y_[i] + z * k1[i];
    }

    vector<double> k2 = f_(x, temp);

    for(int i =0; i < y_.size(); i++)
    {
        temp[i] = y_[i] + z * k2[i];
    }

    vector <double> k3 = f_(x, temp);

    for(int i =0; i < y_.size(); i++)
    {
        temp[i] = y_[i] + h_ * k3[i];
    }
    
    vector <double> k4 = f_(t_ + h_, temp);
// sum up for particular step!
    for(int i =0; i < y_.size(); i++)
    {
        sum[i] = y_[i] + a*k1[i] + 2*a*k2[i] + 2*a*k3[i] + a*k4[i];
    }

    return sum;
}

// returns a vector that we can change as needed!
// can accept any ODE equation
    std::function<vector<double>(double, vector<double>)> f_;

    public:
// add the constructor
// creates object and gives it starting values
    ODE(double t, vector<double>y, double h, int steps, std::function<vector<double>(double, vector<double>)>f ):t_(t), y_(y), h_(h), steps_(steps), f_(f){}

// method to continue solving and print does not need to return!
void solve(){

// prints values for each step
//while updating t to ensure calculations are proper!
    for(int i =0; i < steps_; i++)
    {
        std::cout << "t = "<< t_ << ", x = " <<y_[0] << ", v = " << y_[1] << endl;
        y_ = rk4_step();
        t_ += h_;
    }
}
};

int main()
{
// starting values we need to be given by the user
    double t0;
    double h;
    double g;
    int steps;
    double x0;
    double v0;

    std::cin >> t0 >> h >> steps>>g;
    std::cin >> x0 >> v0;

// going to give us what the function is assigned to!
// lambda approach

    auto my_ode = [g](double t, vector<double>y) -> vector<double>
    {
        return {y[1], -g};
    };

// create the object!
    vector<double> y0 = {x0, v0};
    ODE solver(t0, y0, h, steps, my_ode);
    solver.solve();
    return 0;
}
