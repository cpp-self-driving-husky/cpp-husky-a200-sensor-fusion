#ifndef STATE_VECTOR_H_
#define STATE_VECTOR_H_


namespace state {

    template<class T>
    class StateVector {

        public:
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

            /*
            void addVectorMatrixRow(
                mtx::Matrix& matrix,
                int init_row)
            {
                int cols = matrix.getCols();
                int init_elem = init_row * cols;
                for (int i = 0; i < cols; ++i)
                    this->vec_[i] += matrix[init_elem+i];
            }

            void subVectorMatrixRow(
                mtx::Matrix& matrix,
                int init_row)
            {
                int cols = matrix.getCols();
                int init_elem = init_row * cols;
                for (int i = 0; i < cols; ++i)
                    this->vec_[i] -= matrix[init_elem+i];
            }
            */

            T& operator[](int i) {
                return this->vec_[i];
            }

            int getVars() {
                return this->vars_;
            }

        private:
            T* vec_;
            int vars_;

    };

    template<class T>
    using ControlVector = StateVector<T>;

    template<class T>
    using MeasurementVector = StateVector<T>;

    template<class T>
    using ParameterVector = StateVector<T>;


}


#endif // STATE_VECTOR_H_

