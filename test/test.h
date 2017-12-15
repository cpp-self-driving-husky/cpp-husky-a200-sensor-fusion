#ifndef TEST_H_
#define TEST_H_
#include <string>
//#include "../src/data_matrix.h"
//#include "../src/mean_vector.h"
//#include "../src/covariance_matrix.h"
#include "../src/state_vector.h"
#include "../src/motion_model.h"
#include "../src/matrix.h"


namespace test {

    class Test {

        public:

            //data::DataMatrix testDataMatrixA(bool output=false);
            //vec::MeanVector testMeanVectorA();

            //void testMeanVectorA();
            //void testMeanVectorB(int seed);

            //void testCovarianceMatrix();

            void testMotionModelA();

            void testMatrixA();

            void testMatrixB();

            void testMatrixTranspose();

            void testAddVectorMatrix();


        private:
            //void print_matrix(
            //    std::string message, double* matrix,
            //    int rows, int cols);
            //void print_matrix(
            //    std::string message,
            //    data::DataMatrix& matrix);

            //void print_vector(
            //    std::string message,
            //    vec::MeanVector& vec);

            //void print_matrix(
            //    std::string& message,
            //    cov::CovarianceMatrix& covariance);

            void print_vector(
                std::string msg,
                state::StateVector& xt);

            void print_matrix(
                std::string msg,
                mtx::Matrix& matrix);


    };

}


#endif // TEST_H_


