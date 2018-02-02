#ifndef UNSCENTED_KALMAN_FILTER_H_
#define UNSCENTED_KALMAN_FILTER_H_
#include "state_vector.h"
#include "matrix.h"
#include "sigma_points.h"
#include "motion_model.h"
#include "sensor_model.h"


namespace ukf {

    // Unscented Kalman Filter is generic ukf,
    // ukf for motion model will be implemented
    // as different algorithm, as that would be
    // a more efficient means of implementation
    class UnscentedKalmanFilter {

        public:
            UnscentedKalmanFilter(int state_size);
            ~UnscentedKalmanFilter();

            void init(int state_size);
            void destroy();

            void setMotionModel(model::MotionModel<double>* model);
            void setSensorModel(sensor::SensorModel<double>* sensor);

            double calculateLambda(double alpha, double kappa, int n);
            double calculateGamma(double lambda, int n);
            int pointsPerState(int state_size);

            void matrixTimesTranspose(
                mtx::Matrix<double>& product,
                mtx::Matrix<double>& matrix);

            void calculateSigmaPoints(
                sigma::SigmaPoints<double>& sigma,
                state::StateVector<double>& state,
                mtx::Matrix<double>& covariance);

            void priorFunction(
                sigma::SigmaPoints<double>& prior,
                state::StateVector<double>& control,
                sigma::SigmaPoints<double>& sigma);

            void sumWeighedState(
                state::StateVector<double>& state,
                sigma::SigmaPoints<double>& sigma,
                state::WeightVector<double>& weights);

            void sumWeighedCovariance(
                mtx::Matrix<double>& covariance,
                state::WeightVector<double>& weight,
                sigma::SigmaPoints<double>& sigma,
                state::StateVector<double>& belief,
                mtx::Matrix<double>& noise);

            void predictionFunction(
                sigma::SigmaPoints<double>& observation,
                sigma::SigmaPoints<double>& prediction);

            void update(
                state::StateVector<double>& state_vector,
                mtx::CovarianceMatrix<double>& covariance_matrix,
                state::ControlVector<double>& control_vector,
                state::MeasurementVector<double>& measurement_vector);


        private:
            state::StateVector<double> state_belief_;
            state::StateVector<double> state_observation_;

            state::StateVector<double> blank_;

            mtx::CovarianceMatrix<double> covariance_;
            mtx::CovarianceMatrix<double> covariance_belief_;
            mtx::Matrix<double> observation_uncertainty_;

            mtx::Matrix<double> cholesky_matrix_;
            mtx::Matrix<double> noise_r_;
            mtx::Matrix<double> noise_q_;

            // rename this
            mtx::Matrix<double> holder_;
            mtx::Matrix<double> matrix_transpose_;

            sigma::SigmaPoints<double> sigma_;
            sigma::SigmaPoints<double> sigma_prev_;
            sigma::SigmaPoints<double> sigma_prior_;
            sigma::SigmaPoints<double> sigma_uncertainty_;
            sigma::SigmaPoints<double> sigma_observation_;


            state::WeightVector<double> mean_weights_;
            state::WeightVector<double> covariance_weights_;

            model::MotionModel<double>* motion_model_;
            sensor::SensorModel<double>* sensor_model_;


            int state_size_;
            double lambda_;
            double gamma_;

    };

}






#endif // UNSCENTED_KALMAN_FILTER_H_

