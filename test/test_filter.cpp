#include<iostream>
#include<string>
#include<vector>

#include "../src/unscented_kalman_filter.h"




void testFilterA() {

    int state_size = 5;
    int data_size = 6;

    std::vector<double> data_sample = {
        10,4,21,7,13,
        23,1,6,9,17,
        3,11,5,4,31,
        5,19,24,8,8,
        15,29,11,9,14,
        9,13,5,5,19
    };
    mtx::DataMatrix<double> data_matrix(data_size,state_size);
    for (int i = 0; i < state_size*data_size; ++i)
        data_matrix[i] = data_sample[i];

    // init motion model
    model::MotionModel<double>* motion_model
        = new model::SimpleMotionModel<double>();

    // init unscented kalman filter
    ukf::UnscentedKalmanFilter filter(state_size,data_size);
    filter.setMotionModel(motion_model);

    // init state vector and covariance matrix
    state::StateVector<double> state(state_size);
    data_matrix.mean(state);
    mtx::CovarianceMatrix<double> covariance(state_size,state_size);
    data_matrix.covariance(covariance,state);
    //covariance.print();

    // empty control and measurement vectors
    state::ControlVector<double> control(state_size);
    state::MeasurementVector<double> measurement(state_size);


    filter.update(state,covariance,control,measurement);







    // one iteration of kalman filter
    //filter.update(
    //    state_vector,
    //    covariance_matrix,
    //    control_vector,
    //    measurement_vector);


}





int main(int argc, char* argv[]) {
    testFilterA();
    return 0;
}



