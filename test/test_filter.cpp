#include<iostream>
#include<string>
#include<vector>
#include <chrono>
#include <ctime>
#include "../src/ukf.h"



void populateSigmaA(sigma::SigmaPoints<double>& sigma) {

    std::vector<double> elems = {
        8,7,2,
        4,5,6,
        2,7,2,
        3,4,5,
        9,1,5,
        5,2,6,
        4,7,1
    };
    size_t col = 3;

    for (int i = 0; i < sigma.getNumPoints(); ++i)
        for (int j = 0; j < sigma.getStateSize(); ++j)
            sigma[i][j] = elems[i*col+j];

}


template<typename T>
void replicate(mtx::Matrix<T>& matrix, std::vector<T>& elems) {
    int len = matrix.getSize();
    for (int i = 0; i < len; ++i)
        matrix[i] = elems[i];
}


void populateArrayA(mtx::Matrix<double>& matrix) {

    std::vector<double> elems = {
        2,3,4,6,3,
        1,2,5,4,2,
        2,1,8,2,7,
        3,4,2,3,1,
        5,4,2,2,8,
        3,5,5,9,4,
        5,4,4,5,4,
        2,6,7,5,3
    };
    replicate(matrix,elems);
}


void populateArrayB(mtx::Matrix<double>& matrix) {

    std::vector<double> elems = {
        0.2, 1.1, 3.3,
        0.5, 1.3, 3.2,
        0.7, 1.1, 3.5,
        0.7, 1.4, 3.6,
        0.8, 1.5, 3.7
    };
    replicate(matrix,elems);
}


ukf::UnscentedKalmanFilter<double> initialize(int vars) {
    ukf::UnscentedKalmanFilter<double> ukf(vars);
    return ukf;
}


state::StateVector<double> vec(int vars) {
    state::StateVector<double> state(vars);
    return state;
}


mtx::Matrix<double> mtrx(int vars) {
    mtx::Matrix<double> mtx(vars,vars);
    return mtx;
}


mtx::Matrix<double> mtrx(int rows,int cols) {
    mtx::Matrix<double> mtx(rows,cols);
    return mtx;
}


void testFilterA(int vars, ukf::UnscentedKalmanFilter<double>& ukf) {

    state::StateVector<double> state(vars);
    mtx::Matrix<double> covariance(vars,vars);
    state::StateVector<double> control(vars);
    state::StateVector<double> measure(vars);

    model::MotionModel<double>* motion_model =
        new model::SimpleMotionModel<double>();
    sensor::SensorModel<double>* sensor_model =
        new sensor::SimpleSensorModel<double>();

    ukf.setMotionModel(motion_model);
    ukf.setSensorModel(sensor_model);

    for (int i = 0; i < 10; ++i)
        ukf.update(state,covariance,control,measure);

}


void testFilterB() {

    int vars = 3;
    int obsr = 5;

    auto ukf = initialize(vars);
    auto data = mtrx(obsr,vars);
    auto state = vec(vars);
    auto measure = vec(vars);
    auto control = vec(vars);
    auto covar = mtrx(vars);

    model::MotionModel<double>* motion_model =
        new model::SimpleMotionModel<double>();
    sensor::SensorModel<double>* sensor_model =
        new sensor::SimpleSensorModel<double>();
    ukf.setMotionModel(motion_model);
    ukf.setSensorModel(sensor_model);

    calc::Calculator<double> calc(vars);
    populateArrayB(data);

    calc.mean(state,data);
    calc.covariance(covar,data,state);
    state.print();
    covar.print();

    auto cholesky = mtrx(vars);
    calc.cholesky(cholesky,covar);

    cholesky.print();


    //state.print();
    //covar.print();

    //ukf.update(state,covar,control,measure);

    //state.print();
    //covar.print();

}


int main(int argc, char* argv[]) {

    auto start = std::chrono::system_clock::now();

    testFilterB();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "\n" << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s" << "\n\n";

    return 0;
}



