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

            void update(
                state::StateVector<double>& state_vector,
                mtx::CovarianceMatrix<double>& covariance_matrix,
                state::ControlVector<double>& control_vector,
                state::MeasurementVector<double>& measurement_vector);

            //void updateLocality();

        private:
            state::StateVector<double> state_;
            mtx::CovarianceMatrix<double> covariance_;
            sigma::SigmaPoints<double> sigma_;
            state::WeightVector<double> weights_;

            //mtx::DataMatrix<double> data_matrix_;
            mtx::Matrix<double> cholesky_matrix_;

            model::MotionModel<double>* motion_model_;

            int state_size_;
            double lambda_;
            double gamma_;

    };

}






#endif // UNSCENTED_KALMAN_FILTER_H_

