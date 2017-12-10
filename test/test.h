#ifndef TEST_H_
#define TEST_H_
#include <string>
#include "../src/data_matrix.h"
//#include "../src/mean_vector.h"
//#include "../src/covariance_matrix.h"
#include "../src/state_vector.h"


namespace test {

    class Test {

        public:

            //data::DataMatrix testDataMatrixA(bool output=false);
            //vec::MeanVector testMeanVectorA();

            void testMeanVectorA();
            void testMeanVectorB(int seed);

            //void testCovarianceMatrix();


        private:
            void print_matrix(
                std::string& message, double* matrix,
                int rows, int cols);
            void print_matrix(
                std::string& message,
                data::DataMatrix& matrix);

            void print_vector(
                std::string& message,
                state::MeanVector& vec);

            //void print_matrix(
            //    std::string& message,
            //    cov::CovarianceMatrix& covariance);

    };

}


#endif // TEST_H_


