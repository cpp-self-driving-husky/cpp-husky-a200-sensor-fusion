#ifndef STATE_VECTOR_H_
#define STATE_VECTOR_H_
#include <cmath>
#include <iostream>


namespace state {

    template<class T>
    class StateVector {

        public:

            StateVector() :
                vec_(nullptr), vars_(0)
            {}

            StateVector(int vars) :
                vec_(nullptr), vars_(vars)
            {
                this->init(vars);
            }

            ~StateVector() {
                this->destroy();
            }

            void init(int vars) {
                this->destroy();
                this->vars_ = vars;
                this->vec_ = new T[this->vars_];
                for (int i = 0; i < this->vars_; ++i)
                    this->vec_[i] = 0.0;
            }

            void zero() {
                for (int i = 0; i < this->vars_; ++i)
                    this->vec_[i] = 0.0;
            }

            void replicate(StateVector& state) {
                for (int i = 0; i < this->vars_; ++i)
                    state[i] = this->vec_[i];
            }

            void destroy() {
                if (this->vec_ != nullptr) {
                    delete[] this->vec_;
                    this->vec_ = nullptr;
                }
                this->vars_ = 0;
            }

            int pointsPerState() {
                return 2*this->vars_+1;
            }

            T calculateLambda(double alpha, double kappa) {
                return std::pow(alpha,2)*(this->vars_+kappa)-this->vars_;
            }

            T calculateGamma(double lambda) {
                return std::sqrt(this->vars_+lambda);
            }

            T calculateGamma(double alpha, double kappa) {
                T lambda = this->calculateLambda(alpha,kappa);
                return this->calculateGamma(lambda);
            }

            T& operator[](int i) {
                return this->vec_[i];
            }

            int getVars() {
                return this->vars_;
            }

        protected:
            T* vec_;
            int vars_;

    };


    template<class T>
    using ControlVector = StateVector<T>;

    template<class T>
    using MeasurementVector = StateVector<T>;

    template<class T>
    using ParameterVector = StateVector<T>;


    template<class T>
    class WeightVector : public StateVector<T> {

        public:
            WeightVector(int vars) :
                StateVector<T>(vars)
            {
                this->init(vars);
            }

            ~WeightVector() {
                this->destroy();
            }

            void init(int vars) {
                this->destroy();
                this->vars_ = vars;
                this->points_ = this->pointsPerState();
                this->vec_ = new T[this->points_];
                for (int i = 0; i < this->points_; ++i)
                    this->vec_[i] = 0.0;
            }

            void destroy() {
                if (this->vec_ != nullptr) {
                    delete[] this->vec_;
                    this->vec_ = nullptr;
                }
                this->vars_ = this->points_ = 0;
            }

            T zeroMean(double alpha, double kappa) {
                T lambda = this->calculateLambda(alpha,kappa);
                return lambda / (this->vars_+lambda);
            }

            T zeroCovariance(double alpha, double kappa, double beta) {
                T mean = this->zeroMean(alpha,kappa);
                return mean + (1 - std::pow(alpha,2) + beta);
            }

            T ithElement(double alpha, double kappa) {
                T lambda = this->calculateLambda(alpha,kappa);
                return 1.0 / (2.0 * (lambda + this->vars_));
            }

            void populateMean(double alpha, double kappa) {
                this->vec_[0] = this->zeroMean(alpha,kappa);
                T ith_elem = this->ithElement(alpha,kappa);
                for (int i = 1; i < this->points_; ++i)
                    this->vec_[i] = ith_elem;
            }

            void populateCovariance(double alpha, double kappa, double beta) {
                this->vec_[0] = this->zeroCovariance(alpha,kappa,beta);
                T ith_elem = this->ithElement(alpha,kappa);
                for (int i = 1; i < this->points_; ++i)
                    this->vec_[i] = ith_elem;
            }

            int getPoints() {
                return this->points_;
            }

        private:
            int points_;

    };

}


#endif // STATE_VECTOR_H_



