#ifndef STATE_VECTOR_H_
#define STATE_VECTOR_H_


namespace mtx {
    class Matrix;
    typedef Matrix DataMatrix;
}


namespace state {

    class StateVector {

        public:
            StateVector();
            ~StateVector();

            void init(int vars);
            void init(double* vec, int vars);
            void mean(mtx::DataMatrix& data_matrix);
            double& operator[](int i);
            int getVars();

        protected:
            double* vec_;
            int vars_;

    };

    typedef StateVector ControlVector;
    typedef StateVector MeasurementVector;
    typedef StateVector ParameterVector;

}


#endif // STATE_VECTOR_H_

