#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "../src/state_vector.h"
#include "../src/matrix.h"
#include "../src/sigma_points.h"




void printStateVector(
    std::string msg,
    state::StateVector& xt)
{
    std::cout << "\n" << msg << "\n";
    int vars = xt.getVars();
    for (int i = 0; i < vars; ++i)
        std::cout << xt[i] << " ";
    std::cout << "\n" <<
        "variables " << vars << "\n\n";

}


void printMatrix(std::string msg, mtx::Matrix& matrix) {
    std::cout << "\n" << msg << "\n";
    for (int r = 0; r < matrix.getRows(); ++r) {
        for (int c = 0; c < matrix.getCols(); ++c) {
            std::cout << matrix[r*matrix.getCols()+c] << " ";
        }
        std::cout << "\n";
    }
    std::cout <<
        "rows: " << matrix.getRows() << ", " <<
        "cols: " << matrix.getCols() << "\n";
    std::cout << std::endl;
}


void printSigmaPoints(std::string msg, sigma::SigmaPoints& sigma_points) {
    int num_points = sigma_points.getSize();
    int vars = sigma_points[0].getVars();
    std::cout << "\n" << msg << "\n";
    for (int n = 0; n < num_points; ++n) {
        std::cout << n+1 << ": ";
        for (int i = 0; i < vars; ++i) {
            std::cout << sigma_points[n][i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "size " << num_points << "\n";
    std::cout << std::endl;
}


void printVector(std::string msg, std::vector<double>& vec, int newline=-1) {
    std::cout << "\n" << msg << "\n";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i%newline == 0)
            std::cout << "\n";
        std::cout << vec[i] << " ";
    }
    std::cout << "\n" << std::endl;
}




void testSigmaPointsA() {

    int state_size = 5;
    sigma::SigmaPoints sigma_points(state_size);
    //printSigmaPoints("Sigma Points",sigma_points);

    double alpha = 0.48;
    double kappa = 0.32;
    double lambda = sigma_points.calculateLambda(alpha,kappa,state_size);
    double gamma = sigma_points.calculateGamma(lambda,state_size);
    //std::cout <<
    //    "lambda: " << lambda << ", " <<
    //    "gamma: " << gamma << std::endl;

    // data matrix
    // covariance matrix
    // empty cholesky matrix
    // mean vector

    srand(1);
    int data_rows = 7, data_cols = 5;
    std::vector<double> samples(data_rows*data_cols);
    for (size_t i = 0; i < samples.size(); ++i)
        samples[i] = rand() % 20;

    mtx::Matrix data_matrix(data_rows,data_cols);
    for (int i = 0; i < data_matrix.getSize(); ++i)
        data_matrix[i] = samples[i];
    //printMatrix("Data Matrix",data_matrix);

    state::StateVector state(data_cols);
    state.mean(data_matrix);
    //printStateVector("State Vector",state);

    mtx::CovarianceMatrix covariance(data_cols,data_cols);
    data_matrix.covariance(covariance,state);
    //printMatrix("Covariance Matrix",covariance);

    mtx::Matrix cholesky(data_cols,data_cols);
    //covariance.cholesky(cholesky);
    //printMatrix("Cholesky Matrix",cholesky);
    //std::cout << "gamma " << gamma << "\n";
    //cholesky *= gamma;
    //printMatrix("Cholesky Gamma Matrix",cholesky);
    //for (int i = 0; i < data_cols; ++i)
    //    sigma_points[4][i] = i;

    sigma_points.generatePoints(state,covariance,cholesky,gamma);




    printSigmaPoints("Sigma Points", sigma_points);


}





int main(int argc, char* argv[]) {
    testSigmaPointsA();
    return 0;
}

