#include "unscented_kalman_filter.h"
#include <iostream>
#include <cmath>


namespace {

    const double ALPHA = 0.5;
    const double BETA = 2.0;
    const double KAPPA = 0.5;

}


ukf::UnscentedKalmanFilter::UnscentedKalmanFilter(int n) :

    state_belief_(state::StateVector<double>(n)),
    state_observation_(state::StateVector<double>(n)),

    covariance_(mtx::CovarianceMatrix<double>(n,n)),
    covariance_belief_(mtx::CovarianceMatrix<double>(n,n)),
    observation_uncertainty_(mtx::Matrix<double>(n,n)),
    cross_covariance_(mtx::CovarianceMatrix<double>(n,n)),

    cholesky_matrix_(mtx::Matrix<double>(n,n)),
    prediction_holder_(mtx::Matrix<double>(n,this->pointsPerState(n))),

    prediction_matrix_(mtx::Matrix<double>(n,this->pointsPerState(n))),
    uncertainty_transpose_(mtx::Matrix<double>(this->pointsPerState(n),n)),

    kalman_gain_(mtx::Matrix<double>(n,n)),

    noise_r_(mtx::Matrix<double>(n,n)),
    noise_q_(mtx::Matrix<double>(n,n)),

    sigma_(sigma::SigmaPoints<double>(n)),
    sigma_prev_(sigma::SigmaPoints<double>(n)),
    sigma_prior_(sigma::SigmaPoints<double>(n)),
    sigma_uncertainty_(sigma::SigmaPoints<double>(n)),
    sigma_observation_(sigma::SigmaPoints<double>(n)),

    mean_weights_(state::WeightVector<double>(n)),
    covariance_weights_(state::WeightVector<double>(n)),
    blank_(state::StateVector<double>(n)),

    state_size_(n),
    motion_model_(nullptr),
    sensor_model_(nullptr)


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

    //this->state_size_;

}


void ukf::UnscentedKalmanFilter::destroy() {
    if (this->motion_model_ != nullptr) {
        delete this->motion_model_;
        this->motion_model_ = nullptr;
    }
    if (this->sensor_model_ != nullptr) {
        delete this->sensor_model_;
        this->sensor_model_ = nullptr;
    }
}


void ukf::UnscentedKalmanFilter::setMotionModel(
    model::MotionModel<double>* model)
{
    this->motion_model_ = model;
}


void ukf::UnscentedKalmanFilter::setSensorModel(
    sensor::SensorModel<double>* sensor)
{
    this->sensor_model_ = sensor;
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
    int vars = state.getVars();
    int points = sigma.getSize();
    for (int i = 0; i < points; ++i) {
        for (int j = 0; j < vars; ++j) {
            state[j] = sigma[i][j] * weights[j];
        }
    }
}


// perhaps redo this, could be inefficient
void ukf::UnscentedKalmanFilter::matrixTimesTranspose(
    mtx::Matrix<double>& product,
    mtx::Matrix<double>& matrix)
{
    product.zero();
    int n = matrix.getRows();
    int m = matrix.getCols();
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < m; ++k)
            for (int j = i; j < n; ++j)
                product[i*n+j] += matrix[i*m+k] * matrix[j*m+k];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            product[j*n+i] = product[i*n+j];
}


void ukf::UnscentedKalmanFilter::multiplyMatrices(
    mtx::Matrix<double>& product,
    mtx::Matrix<double>& multiplier,
    mtx::Matrix<double>& multiplicand)
{
    product.zero();
    int rows = multiplier.getRows();
    int inner = multiplier.getCols();
    int cols = multiplicand.getCols();
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            for (int k = 0; k < inner; ++k)
                product[i*cols+j] += multiplier[i*inner+k] * multiplicand[k*cols+j];
}


void ukf::UnscentedKalmanFilter::sumWeighedCovariance(
    mtx::Matrix<double>& covariance,
    state::WeightVector<double>& weight,
    sigma::SigmaPoints<double>& sigma,
    state::StateVector<double>& belief,
    mtx::Matrix<double>& noise)
{
    covariance.zero();
    int vars = sigma.getStateSize();
    int points = sigma.getSize();

    // check to make sure this is correct!
    // may have to transpose it as it is copied!
    for (int i = 0; i < points; ++i)
        for (int j = 0; j < vars; ++j)
            this->prediction_holder_[i*vars+j] = sigma[i][j] - belief[j];

    // could be consolidated with loops above?
    for (int i = 0; i < points; ++i)
        for (int j = 0; j < vars; ++j)
            this->prediction_holder_[i*vars+j] *= weight[i];

    this->matrixTimesTranspose(
        covariance,
        this->prediction_holder_);

    covariance += noise;
}


void ukf::UnscentedKalmanFilter::predictionFunction(
    sigma::SigmaPoints<double>& observation,
    sigma::SigmaPoints<double>& prediction)
{
    int points = prediction.getSize();
    for (int i = 0; i < points; ++i)
        this->sensor_model_->calculate(observation,prediction);
}


void ukf::UnscentedKalmanFilter::calculateCrossCovariance(
    mtx::Matrix<double>& cross_covariance,
    sigma::SigmaPoints<double>& sigma_uncertainty,
    sigma::SigmaPoints<double>& sigma_observation,
    state::StateVector<double>& prediction,
    state::StateVector<double>& observation,
    state::WeightVector<double>& weight)
{
    int vars = sigma_uncertainty.getStateSize();
    int points = sigma_uncertainty.getSize();
    for (int i = 0; i < points; ++i) {
        for (int j = 0; j < vars; ++j) {
            this->prediction_matrix_[j*points+i] = sigma_uncertainty[i][j] - prediction[j];
            this->uncertainty_transpose_[i*vars+j] = sigma_observation[i][j] - prediction[j];
        }
    }

    // can be consolidated with loop above?
    for (int i = 0; i < points; ++i) {
        for (int j = 0; j < vars; ++j) {
            this->prediction_matrix_[j*points+i] *= weight[i];
            this->uncertainty_transpose_[i*vars+j] *= weight[i];
        }
    }

    this->multiplyMatrices(
        cross_covariance,
        this->prediction_matrix_,
        this->uncertainty_transpose_);

}


void ukf::UnscentedKalmanFilter::calculateKalmanGain(
    mtx::Matrix<double>& kalman_gain,
    mtx::Matrix<double>& covariance,
    mtx::Matrix<double>& uncertainty)
{



}



void ukf::UnscentedKalmanFilter::update(
    state::StateVector<double>& state_vector,
    mtx::CovarianceMatrix<double>& covariance_matrix,
    state::ControlVector<double>& control_vector,
    state::MeasurementVector<double>& measurement_vector)
{

    this->calculateSigmaPoints(
        this->sigma_,
        state_vector,
        covariance_matrix);

    this->priorFunction(
        this->sigma_prev_,
        control_vector,
        this->sigma_);

    this->sumWeighedState(
        this->state_belief_,
        this->sigma_prev_,
        this->mean_weights_);

    this->sumWeighedCovariance(
        this->covariance_belief_,
        this->covariance_weights_,
        this->sigma_prev_,
        this->state_belief_,
        this->noise_r_);

    this->calculateSigmaPoints(
        this->sigma_uncertainty_,
        this->state_belief_,
        this->covariance_belief_);

    this->predictionFunction(
        this->sigma_observation_,
        this->sigma_uncertainty_);

    this->sumWeighedState(
        this->state_observation_,
        this->sigma_observation_,
        this->mean_weights_);

    this->sumWeighedCovariance(
        this->observation_uncertainty_,
        this->covariance_weights_,
        this->sigma_observation_,
        this->state_observation_,
        this->noise_q_);

    this->calculateCrossCovariance(
        this->cross_covariance_,
        this->sigma_uncertainty_,
        this->sigma_observation_,
        this->state_belief_,
        this->state_observation_,
        this->covariance_weights_);





    std::cout << "\n" << "end of filter" << "\n" << std::endl;

}






// end of file

