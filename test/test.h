#ifndef TEST_H_
#define TEST_H_
#include <string>
#include "../src/data_matrix.h"
#include "../src/mean_vector.h"


namespace test {

    class Test {

        public:

            //data::DataMatrix testDataMatrixA(bool output=false);
            //vec::MeanVector testMeanVectorA();

            void testMeanVectorA();

        private:
            void print_matrix(
                std::string& message, double* matrix,
                int rows, int cols);
            void print_matrix(
                std::string& message,
                data::DataMatrix& matrix);

            void print_vector(
                std::string& message,
                vec::MeanVector& vec);

    };

}


#endif // TEST_H_


















/*

#pragma once


namespace test {

	class Test {

	public:
		void testCovarianceMatrix();


	private:


	};

}

*/
