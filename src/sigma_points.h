#ifndef SIGMA_POINTS_H_
#define SIGMA_POINTS_H_


namespace state {
    class StateVector;
};


namespace mtx {
    class Matrix;
    typedef Matrix CovarianceMatrix;
};


namespace sigma {

    class SigmaPoints {

        public:
            SigmaPoints(int state_size);
            ~SigmaPoints();

            void init(int state_size);
            void destroy();

            void generatePoints(
                state::StateVector& state,
                mtx::CovarianceMatrix& covariance_matrix,
                mtx::Matrix& cholesky_matrix,
                double gamma);

            double calculateLambda(
                double alpha, double kappa, int n);
            double calculateGamma(double lambda, int n);
            int pointsPerState(int state_size);

            state::StateVector& operator[](int i);
            int getSize();

        private:
            state::StateVector* points_;
            int state_size_;
            int elements_;


    };

}

#endif // SIGMA_POINTS_H_

