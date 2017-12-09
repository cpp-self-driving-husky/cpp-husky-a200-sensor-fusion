#include <iostream>
//#include "covariance_matrix.h"
#include "../test/test.h"


int main(int argc, char* argv[]) {

	//test::Test test;
	//test.testCovarianceMatrix();

	test::Test test;
	//data::DataMatrix matrix = test.testDataMatrixA(false);
	//test.testMeanVectorA(matrix);
	test.testMeanVectorA();

	return 0;
}
