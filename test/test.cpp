#include <iostream>
#include "../src/covariance_matrix.h"
#include "test.h"


void test::Test::testCovarianceMatrix() {

	int rows = 3, cols = 5;
	double* samples = new double[rows*cols];
	samples[0] = 4.0;
	samples[1] = 7.0;
	samples[2] = 9.0;
	samples[3] = 2.0;
	samples[4] = 1.0;
	samples[5] = 5.0;
	samples[6] = 5.0;
	samples[7] = 6.0;
	samples[8] = 8.0;
	samples[9] = 1.0;
	samples[10] = 0.0;
	samples[11] = 2.0;
	samples[12] = 3.0;
	samples[13] = 6.0;
	samples[14] = 8.0;

	cov::CovarianceMatrix covariance_matrix;
	double* covariance = covariance_matrix.calculateCovarianceMatrix(samples, 3, 5);

	// do some sort of comparison here
	for (int i = 0; i < cols*cols; ++i) {
		std::cout << covariance[i] << " ";
		if (i % cols == cols - 1) {
			std::cout << "\n";
		}
	}
	std::cout << std::endl;

}


