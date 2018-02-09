#include <iostream>
#include <string>
//#include "../src/weight_vector.h"
#include "../src/state_vector.h"



void printWeightVector(std::string msg, state::WeightVector<double>& weight) {

    std::cout << "\n" << msg << "\n";
    int vars = weight.getPoints();
    for (int i = 0; i < vars; ++i)
        std::cout << weight[i] << " ";
    std::cout << "\n" << std::endl;

}


void testWeightVectorA() {

    int vars = 5;
    double alpha = 0.5;
    double kappa = 0.2;
    double beta = 2.0;

    state::WeightVector<double> weight_vector(5);
    weight_vector.populateMean(alpha,kappa);
    printWeightVector("Mean Weight Vector",weight_vector);

    weight_vector.populateCovariance(alpha,kappa,beta);
    printWeightVector("Covariance Weight Vector",weight_vector);

}


/*
void testWeightVector() {

    int n = 3;
    double alpha = 1.0, kappa = 0.2;
    double beta = 2.0;
    weight::WeightVector weight(n);
    printWeightVector("Empty Weight Vector",weight);
    double lambda = weight.generateLambda(alpha,kappa,n);
    std::cout << "Lambda: " << lambda << "\n\n";

    weight.calculateWeight(lambda,n,alpha,beta);
    printWeightVector("Filled Weight Vector",weight);

}
*/

int main(int argc, char* argv[]) {
    testWeightVectorA();
    return 0;
}

