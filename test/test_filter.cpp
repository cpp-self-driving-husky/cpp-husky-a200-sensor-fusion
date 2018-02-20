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

    ukf::UnscentedKalmanFilter<double> ukf(vars);
    model::MotionModel<double>* motion = new model::SimpleMotionModel<double>();
    sensor::SensorModel<double>* sensor = new sensor::SimpleSensorModel<double>();
    ukf.setMotionModel(motion);
    ukf.setSensorModel(sensor);

    mtx::Matrix<double> data = fillDataMatrix("../test/data/filter.txt");
    calc::Calculator<double> calc(vars);

    state::StateVector<double> mean(vars);
    mtx::Matrix<double> cov(vars,vars);
    state::StateVector<double> ctrl(vars);
    state::StateVector<double> msr(vars);

    calc.mean(mean,data);
    calc.covariance(cov,data,mean);

    //mean.print();
    //cov.print();
    //ctrl.print();
    //msr.print();

    ukf.update(mean,cov,ctrl,msr);
    ukf.update(mean,cov,ctrl,msr);


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

