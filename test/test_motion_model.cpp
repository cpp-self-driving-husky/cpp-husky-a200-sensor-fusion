#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "../src/motion_model.h"


void printVector(
    std::string msg,
    state::StateVector<double>& xt)
{
    std::cout << "\n" << msg << "\n";
    int vars = xt.getVars();
    for (int i = 0; i < vars; ++i)
        std::cout << xt[i] << " ";
    std::cout << "\n" <<
        "variables " << vars << "\n\n";

}


void testMotionModelA() {

    int vars = 3;
    state::StateVector<double> a(vars);
    state::StateVector<double> b(vars);
    state::StateVector<double> c(vars);
    state::StateVector<double> d(vars);

    for (int i = 0; i < vars; ++i) {

        a[i] = 60;
        b[i] = 59;
        c[i] = 60;
        d[i] = 59;

    }

    //printVector("Vector A",a);
    //printVector("Vector B",b);
    //printVector("Vector C",c);
    //printVector("Vector D",d);

    model::OdometryMotionModel<double> motion_model;
    double probability = motion_model.calculate(a,b,c,d);

    std::cout <<"\n" << "Probability " << probability << "\n" << std::endl;


}


int main(int argc, char* argv[]) {
    testMotionModelA();
    return 0;
}

