#include "unscented_kalman_filter.h"
#include <iostream>
#include <cmath>


namespace {

    const double ALPHA = 0.5;
    const double BETA = 2.0;
    const double KAPPA = 0.5;

}


ukf::UnscentedKalmanFilter::UnscentedKalmanFilter(int n, int samples) :

    state_belief_(state::StateVector<double>(n)),

    covariance_(mtx::CovarianceMatrix<double>(n,n)),
    cholesky_matrix_(mtx::Matrix<double>(n,n)),

    sigma_(sigma::SigmaPoints<double>(n)),
    sigma_prev_(sigma::SigmaPoints<double>(n)),
    sigma_prior_(sigma::SigmaPoints<double>(n)),

    mean_weights_(state::WeightVector<double>(n)),
    covariance_weights_(state::WeightVector<double>(n)),

    blank_(state::StateVector<double>(n)),

    state_size_(n),
    motion_model_(nullptr)
{
    this->init(n);
}


ukf::UnscentedKalmanFilter::~UnscentedKalmanFilter() {
    this->destroy();
}


void ukf::UnscentedKalmanFilter::init(int n) {

    this->lambda_ = this->calculateLambda(ALPHA,KAPPA,n);
    this->gamma_ = this->calculateGamma(this->lambda_,n);

    this->mean_weights_.populateMean(ALPHA,KAPPA);
    this->covariance_weights_.populateCovariance(ALPHA,KAPPA,BETA);

}


void ukf::UnscentedKalmanFilter::destroy() {
    if (this->motion_model_ != nullptr) {
        delete this->motion_model_;
        this->motion_model_ = nullptr;
    }
}


void ukf::UnscentedKalmanFilter::setMotionModel(
    model::MotionModel<double>* model)
{
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


void ukf::UnscentedKalmanFilter::priorFunction(
    sigma::SigmaPoints<double>& previous,
    state::StateVector<double>& control,
    sigma::SigmaPoints<double>& sigma)
{
    int len = sigma.getSize();
    for (int i = 0; i < len; ++i)
        this->motion_model_->calculate(
            previous[i],sigma[i],control,this->blank_);
}


void ukf::UnscentedKalmanFilter::sumWeighedState(
    state::StateVector<double>& state,
    sigma::SigmaPoints<double>& sigma,
    state::WeightVector<double>& weights)
{
    sigma.sumWeighedState(state,weights);
}


void ukf::UnscentedKalmanFilter::sumWeighedCovariance(
    mtx::Matrix<double>& covariance,
    state::WeightVector<double>& weight,
    sigma::SigmaPoints<double>& sigma,
    state::StateVector<double>& belief,
    mtx::Matrix<double>& noise)
{

}



void ukf::UnscentedKalmanFilter::update(
    state::StateVector<double>& state_vector,
    mtx::CovarianceMatrix<double>& covariance_matrix,
    state::ControlVector<double>& control_vector,
    state::MeasurementVector<double>& measurement_vector)
{

    this->calculateSigmaPoints(this->sigma_,state_vector,covariance_matrix);
    this->priorFunction(this->sigma_prev_,control_vector,this->sigma_);
    this->sumWeighedState(this->state_belief_,this->sigma_prev_,this->mean_weights_);

}












// end of file

