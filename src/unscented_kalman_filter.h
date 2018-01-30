#ifndef UNSCENTED_KALMAN_FILTER_H_
#define UNSCENTED_KALMAN_FILTER_H_
#include "state_vector.h"
#include "matrix.h"
#include "sigma_points.h"
#include "motion_model.h"


namespace ukf {

    // Unscented Kalman Filter is generic ukf,
    // ukf for motion model will be implemented
    // as different algorithm, as that would be
    // a more efficient means of implementation
    class UnscentedKalmanFilter {

        public:
            UnscentedKalmanFilter(int state_size, int samples);
            ~UnscentedKalmanFilter();

            void init(int state_size);
            void destroy();

            void setMotionModel(model::MotionModel<double>* model);

            double calculateLambda(double alpha, double kappa, int n);
            double calculateGamma(double lambda, int n);
            int pointsPerState(int state_size);

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


            void update(
                state::StateVector<double>& state_vector,
                mtx::CovarianceMatrix<double>& covariance_matrix,
                state::ControlVector<double>& control_vector,
                state::MeasurementVector<double>& measurement_vector);


        private:
            //state::StateVector<double> state_;
            state::StateVector<double> state_belief_;

            mtx::CovarianceMatrix<double> covariance_;
            mtx::Matrix<double> cholesky_matrix_;
            mtx::Matrix<double> noise_r_;
            mtx::Matrix<double> holder_;

            sigma::SigmaPoints<double> sigma_;
            sigma::SigmaPoints<double> sigma_prev_;
            sigma::SigmaPoints<double> sigma_prior_;

            state::WeightVector<double> mean_weights_;
            state::WeightVector<double> covariance_weights_;

            model::MotionModel<double>* motion_model_;

            state::StateVector<double> blank_;

            int state_size_;
            double lambda_;
            double gamma_;

    };

}






#endif // UNSCENTED_KALMAN_FILTER_H_

