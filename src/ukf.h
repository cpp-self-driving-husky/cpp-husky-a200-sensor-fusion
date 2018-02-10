#ifndef UKF_H_
#define UKF_H_
#include "calculator.h"
#include "sigma_points.h"
#include "motion_model.h"
#include "sensor_model.h"




namespace ukf {

    namespace {
        const double ALPHA = 0.5;
        const double BETA = 2.0;
        const double KAPPA = 0.5;
    }


    template<class T>
    class UnscentedKalmanFilter {

        public:
            UnscentedKalmanFilter(int vars) :
                vars_(0),points_(0),lambda_(0.0),gamma_(0.0),
                motion_model_(nullptr),sensor_model_(nullptr),
                compute_(calc::Calculator<T>(vars)),

                state_belief_(state::StateVector<T>(vars)),
                state_obser_(state::StateVector<T>(vars)),

                covar_belief_(mtx::Matrix<T>(vars,vars)),
                covar_obser_(mtx::Matrix<T>(vars,vars)),
                covar_obser_inv_(mtx::Matrix<T>(vars,vars)),
                covar_cross_(mtx::Matrix<T>(vars,vars)),

                sigma_belief_(this->pointsPerState(vars),vars),
                sigma_predict_(this->pointsPerState(vars),vars),

                mean_weight_(state::WeightVector<T>(vars,this->pointsPerState(vars))),
                covar_weight_(state::WeightVector<T>(vars,this->pointsPerState(vars))),

                noise_r_(mtx::Matrix<T>(vars,vars)),
                noise_q_(mtx::Matrix<T>(vars,vars)),
                kalman_gain_(mtx::Matrix<T>(vars,vars)),

                place_holder_(state::StateVector<T>(vars))
            {}

            ~UnscentedKalmanFilter() {
                this->destroy();
            }

            void init(int vars) {
                this->vars_ = vars;
                this->points_ = this->pointsPerState(this->vars_);
                this->mean_weight_.populateMean(this->lambda_);
                this->covar_weight_.populateCovariance(this->lambda_,ALPHA,BETA);
            }

            void destroy() {
                this->deallocateModel(this->motion_model_);
                this->deallocateModel(this->sensor_model_);
            }

            template<typename S>
            void deallocateModel(S* elem) {
                if (elem != nullptr) {
                    delete elem;
                    elem = nullptr;
                }
            }

            void setMotionModel(model::MotionModel<T>* model) {
                this->deallocateModel(this->motion_model_);
                this->motion_model_ = model;
            }

            void setSensorModel(sensor::SensorModel<T>* sensor) {
                this->deallocateModel(this->sensor_model_);
                this->sensor_model_ = sensor;
            }

            T calculateLambda(T alpha, T kappa, int n) {
                return std::pow(alpha,2)*(n+kappa)-n;
            }

            T calculateGamma(T lambda, int n) {
                return std::sqrt(n+lambda);
            }

            int pointsPerState(int state) {
                return 2*state+1;
            }


            void sigmaPoints(
                sigma::SigmaPoints<T>& sigma,
                state::StateVector<T>& state,
                mtx::Matrix<T>& covariance,
                T gamma)
            {
                this->compute_.cholesky(covariance);
                sigma.generatePoints(state,covariance,gamma);
            }

            void gFunction(
                sigma::SigmaPoints<T>& predict,
                sigma::SigmaPoints<T>& belief,
                state::StateVector<T>& control)
            {
                int points = predict.getNumPoints();
                for (int i = 0; i < points; ++i)
                    this->motion_model_->calculate(
                        predict[i],belief[i],
                        control,this->place_holder_);
            }

            void hFunction(
                sigma::SigmaPoints<T>& predict,
                sigma::SigmaPoints<T>& belief)
            {
                this->sensor_model_->calculate(predict,belief);
            }

            void sumWeightedMean(
                state::StateVector<T>& state,
                sigma::SigmaPoints<T>& sigma,
                state::WeightVector<T>& weights)
            {
                state.zero();
                int points = sigma.getNumPoints(),
                    vars = sigma.getStateSize();
                for (int i = 0; i < points; ++i)
                    for (int j = 0; j < vars; ++j)
                        state[j] += sigma[i][j] * weights[i];
            }

            void sumWeightedCovariance(
                mtx::Matrix<T>& covariance,
                sigma::SigmaPoints<T>& sigma,
                state::StateVector<T>& belief,
                state::WeightVector<T>& weight,
                mtx::Matrix<T>& noise)
            {
                this->sumWeightedCovariance(
                    covariance,
                    sigma,belief,
                    sigma,belief,
                    weight);
                covariance += noise;
            }

            void sumWeightedCovariance(
                mtx::Matrix<T>& covariance,
                sigma::SigmaPoints<T>& sigma_x,
                state::StateVector<T>& state_a,
                sigma::SigmaPoints<T>& sigma_z,
                state::StateVector<T>& state_b,
                state::WeightVector<T>& weight)
            {
                covariance.zero();
                int rows = sigma_x.getStateSize(),
                    inner = sigma_x.getNumPoints(),
                    cols = sigma_z.getStateSize();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        for (int k = 0; k < inner; ++k)
                            covariance[i*cols+j] +=
                                weight[k] *
                                (sigma_x[k][i]-state_a[i]) *
                                (sigma_z[k][j]-state_b[j]);
            }

            void kalmanGain(
                mtx::Matrix<T>& gain,
                mtx::Matrix<T>& covar,
                mtx::Matrix<T>& obser,
                mtx::Matrix<T>& inv)
            {
                this->compute_.inverse(inv,obser);
                this->compute_.multiply(gain,covar,inv);
            }

            void updateState(
                state::StateVector<T>& state,
                state::StateVector<T>& prev,
                state::StateVector<T>& meas,
                state::StateVector<T>& obser,
                mtx::Matrix<T>& gain)
            {
                state.zero();
                int row = gain.getRows(),
                    col = gain.getCols();
                for (int i = 0; i < row; ++i) {
                    for (int j = 0; j < col; ++j)
                        state[i] += gain[i*col+j] * (meas[i]-obser[i]);
                    state[i] += prev[i];
                }
            }

            void updateCovariance(
                mtx::Matrix<T>& covar,
                mtx::Matrix<T>& belief,
                mtx::Matrix<T>& gain,
                mtx::Matrix<T>& obser)
            {
                this->compute_.multiplyABAt(covar,gain,obser);
                this->compute_.subtract(covar,belief,covar);
            }

            void update(
                state::StateVector<T>& state,
                mtx::CovarianceMatrix<T>& covariance,
                state::ControlVector<T>& control,
                state::MeasurementVector<T>& measurement)
            {
                this->sigmaPoints(
                    this->sigma_belief_,state,
                    covariance,this->gamma_);
                this->gFunction(
                    this->sigma_predict_,
                    this->sigma_belief_,
                    control);
                this->sumWeightedMean(
                    this->state_belief_,
                    this->sigma_predict_,
                    this->mean_weight_);
                this->sumWeightedCovariance(
                    this->covar_belief_,
                    this->sigma_predict_,
                    this->state_belief_,
                    this->covar_weight_,
                    this->noise_r_);
                this->sigmaPoints(
                    this->sigma_belief_,
                    this->state_belief_,
                    this->covar_belief_,
                    this->gamma_);
                this->hFunction(
                    this->sigma_predict_,
                    this->sigma_belief_);
                this->sumWeightedMean(
                    this->state_obser_,
                    this->sigma_predict_,
                    this->mean_weight_);
                this->sumWeightedCovariance(
                    this->covar_obser_,
                    this->sigma_predict_,
                    this->state_belief_,
                    this->covar_weight_,
                    this->noise_q_);
                this->sumWeightedCovariance(
                    this->covar_cross_,
                    this->sigma_belief_,
                    this->state_belief_,
                    this->sigma_predict_,
                    this->state_obser_,
                    this->covar_weight_);
                this->kalmanGain(
                    this->kalman_gain_,
                    this->covar_cross_,
                    this->covar_obser_,
                    this->covar_obser_inv_);
                this->updateState(
                    state,
                    this->state_belief_,
                    measurement,
                    this->state_obser_,
                    this->kalman_gain_);
                this->updateCovariance(
                    covariance,
                    this->covar_belief_,
                    this->kalman_gain_,
                    this->covar_obser_);
            }


        private:
            calc::Calculator<T> compute_;

            model::MotionModel<T>* motion_model_;
            sensor::SensorModel<T>* sensor_model_;

            state::StateVector<T> state_belief_;
            state::StateVector<T> state_obser_;

            mtx::Matrix<T> covar_belief_;
            mtx::Matrix<T> covar_obser_;
            mtx::Matrix<T> covar_obser_inv_;
            mtx::Matrix<T> covar_cross_;

            sigma::SigmaPoints<T> sigma_belief_;
            sigma::SigmaPoints<T> sigma_predict_;

            state::WeightVector<T> mean_weight_;
            state::WeightVector<T> covar_weight_;

            mtx::Matrix<T> noise_r_;
            mtx::Matrix<T> noise_q_;
            mtx::Matrix<T> kalman_gain_;

            T lambda_;
            T gamma_;
            int vars_;
            int points_;


            state::StateVector<T> place_holder_;

    };

}


#endif // UKF_H_

