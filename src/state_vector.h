#ifndef STATE_VECTOR_H_
#define STATE_VECTOR_H_
#include <cmath>


// for testing purposes
#include <iostream>
#include <iomanip>


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
                this->zero();
            }

            void zero() {
                for (int i = 0; i < this->vars_; ++i)
                    this->vec_[i] = 0.0;
            }

            void destroy() {
                if (this->vec_ != nullptr) {
                    delete[] this->vec_;
                    this->vec_ = nullptr;
                }
                this->vars_ = 0;
            }

            state::StateVector<T>& operator=(state::StateVector<T>& state) {
                int vars = state.getVars();
                for (int i = 0; i < vars; ++i)
                    this->vec_[i] = state[i];
                return *this;
            }

            T& operator[](int i) {
                return this->vec_[i];
            }

            int getVars() {
                return this->vars_;
            }

            void precisionPrint() {
                int def_prec = 6,
                    prec = 17;
                std::cout << std::setprecision(prec);
                this->print();
                std::cout << std::setprecision(def_prec);
            }

            void print() {
                for (int i = 0; i < this->vars_; ++i)
                    std::cout << this->vec_[i] << " ";
                std::cout << "\n" <<
                    "variables " << this->vars_ << "\n\n";
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
            WeightVector(int vars, int points) :
                StateVector<T>(),
                points_(0)
            {
                this->init(vars,points);
            }

            ~WeightVector() {
                this->destroy();
            }

            void init(int vars, int points) {
                this->destroy();
                this->vars_ = vars;
                this->points_ = points;
                this->vec_ = new T[this->points_];
                this->zero();
            }

            void destroy() {
                if (this->vec_ != nullptr) {
                    delete[] this->vec_;
                    this->vec_ = nullptr;
                }
                this->vars_ = this->points_ = 0;
            }

            void zero() {
                for (int i = 0; i < this->points_; ++i)
                    this->vec_[i] = 0.0;
            }

            T zeroMean(T lambda) {
                return lambda / (this->vars_+lambda);
            }

            T zeroCovariance(T lambda, T alpha, T beta) {
                T mean = this->zeroMean(lambda);
                return mean + (1 - std::pow(alpha,2) + beta);
            }

            T ithElement(T lambda) {
                return 1.0 / (2.0 * (lambda + this->vars_));
            }

            void populateMean(T lambda) {
                this->vec_[0] = this->zeroMean(lambda);
                T ith_elem = this->ithElement(lambda);
                for (int i = 1; i < this->points_; ++i)
                    this->vec_[i] = ith_elem;
            }

            void populateCovariance(T lambda, T alpha, T beta) {
                this->vec_[0] = this->zeroCovariance(lambda,alpha,beta);
                T ith_elem = this->ithElement(lambda);
                for (int i = 1; i < this->points_; ++i)
                    this->vec_[i] = ith_elem;
            }

            int getNumPoints() {
                return this->points_;
            }

            void print() {
                for (int i = 0; i < this->points_; ++i)
                    std::cout << this->vec_[i] << " ";
                std::cout << "\n" <<
                    "variables: " << this->vars_ << " " <<
                    "points: " << this->points_ << "\n" << std::endl;
            }

        private:
            int points_;

    };

}


#endif // STATE_VECTOR_H_

