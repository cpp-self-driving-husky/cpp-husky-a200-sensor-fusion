#include "utilitest.h"
#include "../src/calculator.h"




void testCalculatorA() {

    int row = 3, col = 4;
    int inner = col;
    calc::Calculator<double> calc(row,col);

    mtx::Matrix<double> A(row,inner);
    mtx::Matrix<double> B(inner,col);
    mtx::Matrix<double> P(row,row);

    std::vector<double> a = {
        6,5,7,3,
        7,3,9,8,
        2,5,1,4
    };
    std::vector<double> b = {
        1,7,4,9,
        6,5,1,6,
        7,8,4,1,
        1,2,7,4
    };


    for (int i = 0; i < A.getSize(); ++i)
        A[i] = a[i];
    for (int i = 0; i < B.getSize(); ++i)
        B[i] = b[i];


    calc.multiplyABAt(P,A,B);

    A.precisionPrint();
    B.precisionPrint();
    P.precisionPrint();

}



void testCalculatorB() {

    int row = 5, col = 3;
    int inner = col;
    calc::Calculator<double> calc(row,col);

    mtx::Matrix<double> A(row,inner);
    mtx::Matrix<double> B(inner,col);
    mtx::Matrix<double> P(row,row);

    std::vector<double> a = {
        6,5,7,
        3,7,3,
        9,8,2,
        5,1,4,
        7,2,3
    };
    std::vector<double> b = {
        1,7,4,
        9,6,5,
        1,6,7,
        8,4,1,
        1,2,7,4
    };


    for (int i = 0; i < A.getSize(); ++i)
        A[i] = a[i];
    for (int i = 0; i < B.getSize(); ++i)
        B[i] = b[i];


    calc.multiplyABAt(P,A,B);

    A.precisionPrint();
    B.precisionPrint();
    P.precisionPrint();

}




void testCovariance() {

    auto matrices = test::processMatrices("data/covariance.txt");
    test::trial("Covariance Matrices");
    for (size_t i = 0; i < matrices.size(); ++i) {

        int cols = matrices[i].first.getCols();
        state::StateVector<double> mean(cols);
        mtx::Matrix<double> cov(cols,cols);
        calc::Calculator<double> calc(cols,cols);

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
        calc::Calculator<double> calc(cols,cols);

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
        calc::Calculator<double> calc(cols,cols);
        calc.inverse(inv,matrices[i].first);

        test::run(inv,matrices[i].second);
    }

}



int main(int argc, char* argv[]) {

    testCovariance();
    testCholesky();
    testInverse();

    testCalculatorA();
    testCalculatorB();

    return 0;

}




