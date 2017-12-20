#include <iostream>
#include <string>
#include "../src/weight_vector.h"


void printWeightVector(std::string msg, weight::WeightVector& weight) {

    std::cout << "\n" << msg << "\n";

    std::cout << "\n" << "Mean Weights" << "\n";
    int vars = weight.getVars();
    for (int i = 0; i < vars; ++i)
        std::cout << weight.ithMean(i) << " ";

    std::cout << "\n\n" << "Covariance Weights" << "\n";
    for (int i = 0; i < vars; ++i)
        std::cout << weight.ithCovariance(i) << " ";
    std::cout << "\n\n" <<
        "size: " << vars << "\n" << std::endl;

}


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


int main(int argc, char* argv[]) {
    testWeightVector();
    return 0;
}

