#ifndef SIGMA_POINTS_H_
#define SIGMA_POINTS_H_
#include <cmath>
#include "matrix.h"


namespace sigma {

    template<class T>
    class SigmaPoints {

        public:
            SigmaPoints(int state_size) :
                points_(nullptr),
                state_size_(state_size),
                elements_(0)
            {
                this->init(state_size);
            }

            ~SigmaPoints() {
                this->destroy();
            }

            void init(int state_size) {
                this->destroy();
                this->state_size_ = state_size;
                this->elements_ = this->pointsPerState(this->state_size_);
                this->points_ = new state::StateVector<T>[this->elements_];
                for (int i = 0; i < this->elements_; ++i)
                    this->points_[i].init(this->state_size_);
            }

            void destroy() {
                if (this->points_ != nullptr) {
                    delete[] this->points_;
                    this->points_ = nullptr;
                }
                this->state_size_ = this->elements_ = 0;
            }

            void generatePoints(
                state::StateVector<T>& state,
                mtx::CovarianceMatrix<T>& covariance_matrix,
                mtx::Matrix<T>& cholesky_matrix,
                double gamma)
            {
                covariance_matrix.cholesky(cholesky_matrix);
                cholesky_matrix *= gamma;
                int vars = state.getVars();
                for (int n = 0; n < this->elements_; ++n)
                    for (int i = 0; i < vars; ++i)
                        this->points_[n][i] = state[i];
                int offset = 1;
                for (int n = offset; n < this->state_size_+1; ++n)
                    cholesky_matrix.addVectorMatrixRow(this->points_[n],n-offset);
                offset = this->state_size_+1;
                for (int n = offset; n < this->elements_; ++n)
                    cholesky_matrix.subVectorMatrixRow(this->points_[n],n-offset);
            }

            // TODO test this!
            void sumWeighedState(
                state::StateVector<T>& state,
                state::WeightVector<T>& weights)
            {
                state.zero();
                int state_size = state.getVars();
                for (int i = 0; i < this->elements_; ++i)
                    for (int j = 0; j < state_size; ++j)
                        state[j] += this->points_[i][j] * weights[i];
            }

            T calculateLambda(double alpha, double kappa) {
                return this->points_[0].calculateLambda(alpha,kappa);
            }

            T calculateGamma(double alpha, double kappa) {
                return this->points_[0].calculateGamma(alpha,kappa);
            }

            int pointsPerState(int state_size) {
                return state_size*2+1;
            }

            state::StateVector<T>& operator[](int i) {
                return this->points_[i];
            }

            int getSize() {
                return this->elements_;
            }

        private:
            state::StateVector<T>* points_;
            int state_size_;
            int elements_;


    };

}

#endif // SIGMA_POINTS_H_

