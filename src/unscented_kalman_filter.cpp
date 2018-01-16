#include "unscented_kalman_filter.h"
#include <iostream>
#include <cmath>


namespace {

    const double ALPHA = 0.5;
    const double BETA = 2.0;
    const double KAPPA = 0.5;

}


ukf::UnscentedKalmanFilter::UnscentedKalmanFilter(int n, int samples) :
    state_(state::StateVector<double>(n)),
    covariance_(mtx::CovarianceMatrix<double>(n,n)),
    sigma_(sigma::SigmaPoints<double>(n)),
    weights_(state::WeightVector<double>(n)),
    //data_matrix_(mtx::DataMatrix<double>(samples,n)),
    cholesky_matrix_(mtx::Matrix<double>(n,n)),
    state_size_(n),
    motion_model_(nullptr)
{
    this->lambda_ = this->calculateLambda(ALPHA,KAPPA,n);
    this->gamma_ = this->calculateGamma(this->lambda_,n);
}


ukf::UnscentedKalmanFilter::~UnscentedKalmanFilter() {
    this->destroy();
}


void ukf::UnscentedKalmanFilter::init(int n) {

}


void ukf::UnscentedKalmanFilter::destroy() {
    if (this->motion_model_ != nullptr) {
        delete this->motion_model_;
        this->motion_model_ = nullptr;
    }
}


void ukf::UnscentedKalmanFilter::setMotionModel(model::MotionModel<double>* model) {
    this->motion_model_ = model;
}


double ukf::UnscentedKalmanFilter::calculateLambda(
    double alpha, double kappa, int n)
{
    return std::pow(alpha,2)*(n+kappa)-n;
}


double ukf::UnscentedKalmanFilter::calculateGamma(double lambda, int n) {
    return std::sqrt(n+lambda);
}


int ukf::UnscentedKalmanFilter::pointsPerState(int state_size) {
    return state_size*2+1;
}


void ukf::UnscentedKalmanFilter::calculateSigmaPoints(
    sigma::SigmaPoints<double>& sigma,
    state::StateVector<double>& state,
    mtx::Matrix<double>& covariance)
{
    sigma.generatePoints(
        state,covariance,
        this->cholesky_matrix_,
        this->gamma_);
}


void ukf::UnscentedKalmanFilter::update(
    state::StateVector<double>& state_vector,
    mtx::CovarianceMatrix<double>& covariance_matrix,
    state::ControlVector<double>& control_vector,
    state::MeasurementVector<double>& measurement_vector)
{
    std::cout << "\n" << "Unscented Kalman Filter update" << "\n" << std::endl;
    this->calculateSigmaPoints(this->sigma_,state_vector,covariance_matrix);



}












// end of file

