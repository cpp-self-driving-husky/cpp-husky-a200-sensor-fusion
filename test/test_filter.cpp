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
    model::MotionModel<double>* motion_model =
        new model::SimpleMotionModel<double>();
    sensor::SensorModel<double>* sensor_model =
        new sensor::SimpleSensorModel<double>();

    // init unscented kalman filter
    ukf::UnscentedKalmanFilter filter(state_size);
    filter.setMotionModel(motion_model);
    filter.setSensorModel(sensor_model);

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




}



void testMatrixTimesTranspose() {

    int rows = 3, cols = 5;

    mtx::Matrix<double> product(rows,rows);
    mtx::Matrix<double> matrix(rows,cols);

    for (int i = 0; i < rows*cols; ++i)
        matrix[i] = i*cols % 9;

    matrix.print();

    ukf::UnscentedKalmanFilter filter(rows);
    filter.matrixTimesTranspose(product,matrix);

    //matrix.print();
    product.print();


}


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




int main(int argc, char* argv[]) {
    //testFilterA();
    testMatrixMultiply();
    return 0;
}



