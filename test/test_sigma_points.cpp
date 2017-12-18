#include <iostream>
#include <string>
#include "../src/sigma_points.h"
#include "../src/state_vector.h"


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


void testSigmaPointsA() {

    int state_size = 5;
    sigma::SigmaPoints sigma_points_(state_size);
    printSigmaPoints("Sigma Points",sigma_points_);

    //int num_points = sigma_points_.getSize();



}





int main(int argc, char* argv[]) {
    testSigmaPointsA();
    return 0;
}

