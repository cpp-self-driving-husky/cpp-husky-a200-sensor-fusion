//#include "utilitest.h"
//#include "../src/calculator.h"
#include "../src/ukf.h"
#include "../src/motion_model.h"
#include "../src/sensor_model.h"
#include <vector>
#include <chrono>
#include <iostream>


/*

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


void testSigmaVariance() {

    int vars = 3;
    int meas = 2*vars+1;

    std::vector<double> inputA = {
        5,7,2,3,1,2,6,
        3,2,6,5,2,3,7,
        4,3,6,2,6,5,4
    };
    std::vector<double> inputB = {
        3,2,5,4,3,5,1,
        5,4,7,3,4,5,3,
        1,3,5,2,4,6,6
    };

    mtx::Matrix<double> matA(vars,meas);
    mtx::Matrix<double> matB(meas,vars);
    mtx::Matrix<double> matP(vars,vars);
    mtx::Matrix<double> matQ(vars,vars);
    mtx::Matrix<double> q(vars,vars);

    sigma::SigmaPoints<double> sigA(meas,vars);
    sigma::SigmaPoints<double> sigB(meas,vars);

    for (int i = 0; i < inputA.size(); ++i) {
        int row = i / vars,
            col = i % vars;

        //matA[i] = inputA[i];
        //matB[i] = inputB[i];

        matA.elem(col,row) = inputA[i];
        matB.elem(col,row) = inputB[i];

        sigA[row][col] = inputA[i];
        sigB[row][col] = inputB[i];
    }

    calc::Calculator<double> calc(vars,meas);
    state::StateVector<double> state(vars);
    state::WeightVector<double> weight(vars,meas);
    for (int i = 0; i < weight.getNumPoints(); ++i) {
        weight[i] = 1.0;
    }
    ukf::UnscentedKalmanFilter<double> ukf(vars,meas);


    matA.print();
    //matB.print();
    sigA.print();
    //sigB.print();


    //ukf.sumWeightedCovariance(matP,sigA,state,weight,q);
    ukf.sumWeightedCovariance(matP,sigA,state,sigA,state,weight);
    calc.multiplyABt(matQ,matA,matA);

    matP.print();
    matQ.print();

}

*/



void testKalmanTrivial() {

    int iterations = 10;
    int state_size = 4;
    int ctrl_size = 3;
    int msr_size = 2;

    double x_i = 78.6;
    double y_i = 191.2;

    double x_d = 0.7;
    double y_d = 0.5;
    double t_d = 1.0;

    std::vector<vct::State<double> > ctrls;
    std::vector<vct::State<double> > msrs;

    for (int i = 0; i < iterations; ++i) {

        ctrls.push_back(vct::State<double>(ctrl_size));
        ctrls[i](0) = x_d;
        ctrls[i](1) = y_d;
        ctrls[i](2) = t_d;

        msrs.push_back(vct::State<double>(msr_size));
        msrs[i](0) = x_i + x_d * (i+1);
        msrs[i](1) = y_i + y_d * (i+1);

    }

    model::ProcessModel<double>* vel_motion =
        new model::SimpleVelocityModel<double>();
    model::SensorModel<double>* vel_sensor =
        new model::SimpleSensorVelocityModel<double>();

    ukf::UnscentedKalmanFilter<double> ukf(state_size,msr_size);
    ukf.setProcessModel(vel_motion);
    ukf.setSensorModel(vel_sensor);

    vct::State<double> state(state_size);
    state(0) = x_i;
    state(1) = y_i;
    state(2) = x_d / t_d;
    state(3) = y_d / t_d;

    mtx::Matrix<double> covar(state_size,state_size);

    //for (int i = 0; i < covar.getSize(); ++i) {
    //    covar[i] = 0.2;
    //}

    //int nrows = covar.getRows();
    //for (int i = 0; i < nrows; ++i) {
    //    covar[i*nrows+i] = 1.0;
    //}

    for (int r = 0; r < covar.rows(); ++r) {
        for (int c = 0; c < covar.cols(); ++c) {
            if (r != c)
                covar(r, c) = 0.2;
            else
                covar(r, c) = 1.0;
        }
    }

    // start now
    std::chrono::steady_clock::time_point init_time = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i) {
        std::cout << "Iteration " << i+1 << "\n" << std::endl;
        ukf.update(state,covar,ctrls[i],msrs[i]);
        std::cout << state << std::endl;
        std::cout << covar << std::endl;
    }

    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    typedef std::chrono::duration<double,std::milli> millisecs_t;
    millisecs_t duration( std::chrono::duration_cast<millisecs_t>(end_time-init_time));
    std::cout << "\n\n" << duration.count() << " milliseconds.\n\n";

}


/*

void testKalmanMeasurements() {

    std::vector<state::StateVector<double> > measurements =
        test::measurements("data/measurements.txt");

    for (int i = 0; i < measurements.size(); ++i)
        measurements[0].precisionPrint();


}

*/


int main(int argc, char* argv[]) {

    //testCovariance();
    //testCholesky();
    //testInverse();

    //testCalculatorA();
    //testCalculatorB();

    testKalmanTrivial();

    //testSigmaVariance();

    //testKalmanMeasurements();

    return 0;

}




