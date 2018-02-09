#include <iostream>
#include <string>
#include "../src/state_vector.h"



void testStateVectorA() {

    int vars = 5;
    state::StateVector<double> state(vars);
    for (int i = 0 ; i < vars; ++i)
        state[i] = i;
    state.print();

}


void testWeightVectorA() {

    int vars = 5;
    int points = 2*vars+1;
    double alpha = 0.5, beta = 2.0, lambda = 1.0;

    state::WeightVector<double> mean(vars,points);
    mean.populateMean(lambda);

    state::WeightVector<double> covar(vars,points);
    covar.populateCovariance(lambda,alpha,beta);

    mean.print();
    covar.print();


}


int main(int argc, char* argv[]) {
    testWeightVectorA();
    return 0;
}





