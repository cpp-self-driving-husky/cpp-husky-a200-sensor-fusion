#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "../src/state_vector.h"
#include "../src/matrix.h"
#include "../src/sigma_points.h"


void printStateVector(
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


void printMatrix(std::string msg, mtx::Matrix<double>& matrix) {
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


void printSigmaPoints(std::string msg, sigma::SigmaPoints<double>& sigma_points) {
    int num_points = sigma_points.getNumPoints();
    int vars = sigma_points.getStateSize();
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


void testSigmaPointsB() {

    int vars = 5;
    int points = 2*vars+1;

    state::StateVector<double> state(vars);
    mtx::Matrix<double> cholesky(vars,vars);
    sigma::SigmaPoints<double> sigma(points,vars);

    int len = cholesky.getSize();
    for (int i = 0 ; i < len; ++i)
        cholesky[i] = (i%vars)+100;

    for (int i = 0; i < vars; ++i)
        state[i] = i/10.0;

    double gamma = 1;
    sigma.SigmaPoints::generatePoints(state,cholesky,gamma);

    state.print();
    cholesky.print();
    sigma.print();

}


int main(int argc, char* argv[]) {
    testSigmaPointsB();
    testSigmaPointsB();
    return 0;
}

