#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../src/ukf.h"
#include "utilitest.h"



mtx::Matrix<double> fillDataMatrix(std::string filepath) {
    std::vector<std::string> lines = test::readlines(filepath);
    mtx::Matrix<double> data = test::processMatrix(lines[0]);
    return data;
}


void testUnscentedKalmanFilter() {

    int vars = 5;
    int samples = 9;
    int meas = 2;

    ukf::UnscentedKalmanFilter<double> ukf(vars,meas);
    model::ProcessModel<double>* motion = new model::SimpleMotionModel<double>();
    model::SensorModel<double>* sensor = new model::SimpleSensorModel<double>();
    ukf.setProcessModel(motion);
    ukf.setSensorModel(sensor);

    mtx::Matrix<double> data = fillDataMatrix("../test/data/filter.txt");

    state::StateVector<double> mean(vars);
    mtx::Matrix<double> cov(vars,vars);
    state::StateVector<double> ctrl(vars);
    state::StateVector<double> msr(meas);

    ukf.mean(mean,data);
    ukf.covariance(cov,data,mean);

    msr[0] = mean[0];
    msr[1] = mean[1];

    mean.precisionPrint();
    cov.precisionPrint();


    for (int i = 0; i < 10; ++i) {

        ukf.covariance(cov,data,mean);
        ukf.update(mean,cov,ctrl,msr);
        std::cout << "iteration " << i << std::endl;
        mean.precisionPrint();
        cov.precisionPrint();

    }


    mean.precisionPrint();
    cov.precisionPrint();





    /*

    std::cout << "initially:" << std::endl;
    mean.precisionPrint();
    cov.precisionPrint();
    for (int i = 0; i < 10; ++i) {
        std::cout << "for iteration " << i << ":\n";
        ukf.update(mean,cov,ctrl,msr);
        mean.precisionPrint();
        cov.precisionPrint();
        std::cout << std::endl;
    }
    std::cout << "\n\nfinal result:" << std::endl;
    mean.precisionPrint();
    cov.precisionPrint();

    */


    //mean.print();
    //cov.print();
    //ctrl.print();
    //msr.print();

}


int main(int argc, char* argv[]) {
    testUnscentedKalmanFilter();
    return 0;
}

