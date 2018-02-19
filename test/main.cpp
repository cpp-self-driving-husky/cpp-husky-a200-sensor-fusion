#include "utilitest.h"
#include "../src/calculator.h"







void testCovariance() {

    auto matrices = test::processMatrices("data/covariance.txt");
    test::trial("Covariance Matrices");
    for (size_t i = 0; i < matrices.size(); ++i) {

        int cols = matrices[i].first.getCols();
        state::StateVector<double> mean(cols);
        mtx::Matrix<double> cov(cols,cols);
        calc::Calculator<double> calc(cols);

        calc.mean(mean,matrices[i].first);
        calc.covariance(cov,matrices[i].first,mean);

        test::run(cov,matrices[i].second);

    }

}


void testCholesky() {

    auto matrices = test::processMatrices("data/cholesky.txt");
    test::trial("Cholesky Matrices");
    for (size_t i = 0; i < matrices.size(); ++i) {

        int cols = matrices[i].first.getCols();
        state::StateVector<double> mean(cols);
        mtx::Matrix<double> cov(cols,cols);
        mtx::Matrix<double> chol(cols,cols);
        calc::Calculator<double> calc(cols);

        calc.mean(mean,matrices[i].first);
        calc.covariance(cov,matrices[i].first,mean);
        calc.cholesky(chol,cov);

        test::run(chol,matrices[i].second);

    }

}


void testInverse() {

    auto matrices = test::processMatrices("data/inverse.txt");
    test::trial("Inverse Matrix");
    for (size_t i = 0; i < matrices.size(); ++i) {

        int cols = matrices[i].first.getCols();
        mtx::Matrix<double> inv(cols,cols);
        calc::Calculator<double> calc(cols);
        calc.inverse(inv,matrices[i].first);

        test::run(inv,matrices[i].second);
    }

}






int main(int argc, char* argv[]) {

    testCovariance();
    testCholesky();
    testInverse();

    return 0;

}
