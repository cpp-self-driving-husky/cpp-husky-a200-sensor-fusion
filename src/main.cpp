#include <iostream>
//#include "covariance_matrix.h"
#include "../test/test.h"


int main(int argc, char* argv[]) {

	test::Test test;
	//test.testMatrixA();
	//test.testMatrixB();
	//test.testMotionModelA();
	//test.testMatrixTranspose();
	test.testAddVectorMatrix();

	return 0;
}
