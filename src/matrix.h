#ifndef MATRIX_H_
#define MATRIX_H_


namespace data {
    class DataMatrix;
}

namespace state {
    class StateVector;
}


namespace mtx {

    class Matrix {

        public:
            Matrix();
            ~Matrix();

            void init(int rows, int cols);
            double elem(int row, int col);
            double& operator[](int i);
            void stateCol(
                state::StateVector& state,
                int col);
            void stateRow(
                state::StateVector& state,
                int row);

            int getRows();
            int getCols();
            int getSize();

        private:
            double* matrix_;
            int rows_;
            int cols_;

    };


    class MatrixCalculator {

        public:
            MatrixCalculator();
            ~MatrixCalculator();

            void cholesky(Matrix& A, Matrix& L);
            void covariance(
                Matrix& covariance_matrix,
                state::StateVector& mean_vector,
                data::DataMatrix& data_matrix);

            //friend operator*=(double scalar, Matrix& matrix);

        private:


    };

}


#endif

