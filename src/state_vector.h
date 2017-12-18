#ifndef STATE_VECTOR_H_
#define STATE_VECTOR_H_


namespace mtx {
    class Matrix;
    typedef Matrix DataMatrix;
}


namespace state {

    class StateVector {

        public:
            StateVector(int vars);
            ~StateVector();

            void init(int vars);
            void replicate(StateVector& state);
            void destroy();

            void mean(mtx::DataMatrix& data_matrix);
            void addVectorMatrixRow(
                mtx::Matrix& matrix,
                int init_row);
            void subVectorMatrixRow(
                mtx::Matrix& matrix,
                int init_row);

            double& operator[](int i);
            int getVars();

        protected:
            double* vec_;
            int vars_;

    };

    typedef StateVector ControlVector;
    typedef StateVector MeasurementVector;
    typedef StateVector ParameterVector;


    /*
    class VectorCalculator {

        public:
            VectorCalculator();
            ~VectorCalculator();

            void addVectorMatrixRow(
                StateVector& state,
                mtx::Matrix& matrix,
                int init_row);
            void subVectorMatrixRow(
                StateVector& state,
                mtx::Matrix& matrix,
                int init_row);


        private:

    };
    */

}


#endif // STATE_VECTOR_H_

