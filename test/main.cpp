//#include "../src/calculator.h"
#include "../src/ukf.h"
#include "../src/process_model.h"
#include "../src/measurement_model.h"
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <iomanip>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>


// #include "utilitest.h"



namespace test {

    static std::vector<std::string> split(const std::string& s, char delimiter) {
       std::vector<std::string> tokens;
       std::string token;
       std::istringstream tokenStream(s);
       while (std::getline(tokenStream, token, delimiter))
          tokens.push_back(token);
       return tokens;
    }


    static std::vector<std::string> readlines(std::string path) {
        std::ifstream file(path);
        std::string line;
        std::vector<std::string> lines;
        if (file.is_open()) {
            while (std::getline(file,line))
                lines.push_back(line);
            file.close();
        }
        return lines;
    }


    // TODO ensure that reading first digit
    //      can occur regardless of size
    //      of previous digit
    static int getDimension(std::string& data, int index) {
        int len = data.size(),
            sum = 0;
        for (int i = index; i < len && data[i] != '/'; ++i)
            sum = sum*10 + data[i]-'0';
        return sum;
    }


    static std::vector<vct::State<double> >
        measurements(std::string filepath, int vars)
    {
        std::vector<std::string> lines = readlines(filepath);
        std::vector<vct::State<double> > meas;
        //std::cout << "\n\n" << std::endl;
        for (int i = 0; i < lines.size(); ++i) {
            vct::State<double> state(vars);
            std::vector<std::string> elems = split(lines[i],',');
            for (int j = 0; j < elems.size(); ++j)
                state[j] = stod(elems[j]);
            meas.push_back(state);
        }
        return meas;
    }

}


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




void testKalmanMeasurements() {

    int state_size = 4;
    int ctrl_size = 1;
    int msr_size = 2;

    std::vector<vct::State<double> > measurements =
        test::measurements("data/raw_odom_9-13_filtered.csv", 3);

    ukf::UnscentedKalmanFilter<double> ukf(state_size,msr_size);

    model::ProcessModel<double>* vel_motion =
        new model::SimpleVelocityModel<double>();
    model::SensorModel<double>* vel_sensor =
        new model::SimpleSensorVelocityModel<double>();
    ukf.setProcessModel(vel_motion);
    ukf.setSensorModel(vel_sensor);

    vct::State<double> state(state_size);
    state[0] = measurements[0](0);
    state[1] = measurements[0](1);
    state[2] = 0.0;
    state[3] = 0.0;

    mtx::Matrix<double> covar(state_size,state_size);
    for (int r = 0; r < covar.rows(); ++r) {
        for (int c = 0; c < covar.cols(); ++c) {
            if (r != c)
                covar(r, c) = 0.2;
            else
                covar(r, c) = 1.0;
        }
    }

    vct::Control<double> control(ctrl_size);
    vct::Measurement<double> measure(msr_size);

    std::cout << "\n" <<
        "initial state" << "\n" << state << "\n\n" <<
        "initial covariance" << "\n" << covar << "\n" << std::endl;

    for (int i = 0; i < measurements.size(); ++i) {
        measure(0) = measurements[i](0);
        measure(1) = measurements[i](1);
        control(0) = measurements[i](2);
        ukf.update(state, covar, control, measure);
    }

    std::cout << "\n" <<
        "final state" << "\n" << state << "\n\n" <<
        "final covariance" << "\n" << covar << "\n" << std::endl;

}





int main(int argc, char* argv[]) {

    //testCovariance();
    //testCholesky();
    //testInverse();

    //testCalculatorA();
    //testCalculatorB();


    //testSigmaVariance();



    //testKalmanTrivial();


    testKalmanMeasurements();

    return 0;

}




