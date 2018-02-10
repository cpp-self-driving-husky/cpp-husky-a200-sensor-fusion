#include<iostream>
#include<string>
#include<vector>
#include <chrono>
#include <ctime>
#include "../src/ukf.h"


/*

void testMatrixMultiply() {

    std::vector<double> elems_a = {
        4,5,1,7,8,
        1,2,0,4,5,
        1,2,3,4,7
    };

    std::vector<double> elems_b = {
        8,7,2,
        4,5,6,
        2,7,2,
        3,4,5,
        9,1,5
    };

    int row_a = 3, col_a = 5;
    int row_b = 5, col_b = 3;
    int row_p = 3, col_p = 3;

    mtx::Matrix<double> product(row_p,col_p);
    mtx::Matrix<double> multiplier(row_a,col_a);
    mtx::Matrix<double> multiplicand(row_b,col_b);

    for (int i = 0; i < row_a*col_a; ++i) {
        multiplier[i] = elems_a[i];
        multiplicand[i] = elems_b[i];
    }

    ukf::UnscentedKalmanFilter filter(row_p);
    filter.multiplyMatrices(product,multiplier,multiplicand);

    product.print();

}

*/


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



void testWeightedCovarianceA() {


    int vars = 3;
    int points = 2*vars+1;
    ukf::UnscentedKalmanFilter<double> filter(vars);

    mtx::Matrix<double> covariance(vars,vars);
    mtx::Matrix<double> noise(vars,vars);
    state::WeightVector<double> weight(vars,points);
    state::StateVector<double> state(vars);
    sigma::SigmaPoints<double> sigma(points,vars);
    populateSigmaA(sigma);

    covariance.print();
    sigma.print();
    filter.sumWeightedCovariance(covariance,sigma,state,weight,noise);
    covariance.print();


}


ukf::UnscentedKalmanFilter<double> initialize(int vars) {
    ukf::UnscentedKalmanFilter<double> ukf(vars);
    return ukf;
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



int main(int argc, char* argv[]) {

    int vars = 500;
    ukf::UnscentedKalmanFilter<double> ukf = initialize(vars);

    auto start = std::chrono::system_clock::now();

    testFilterA(vars,ukf);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "\n" << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s" << "\n\n";

    return 0;
}



