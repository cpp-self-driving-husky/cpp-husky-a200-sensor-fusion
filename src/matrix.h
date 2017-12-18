#ifndef MATRIX_H_
#define MATRIX_H_


namespace state {
    class StateVector;
}


namespace mtx {

    class Matrix {

        public:
            Matrix(int rows, int cols);
            ~Matrix();

            void init(int rows, int cols);
            void replicate(Matrix& matrix);
            void destroy();

            double elem(int row, int col);
            double& operator[](int i);
            void stateCol(
                state::StateVector& state,
                int col);
            void stateRow(
                state::StateVector& state,
                int row);

            void addData(double* data, int meas);
            void removeData(int meas);
            void operator*=(double scalar);

            void transpose(Matrix& T);
            void covariance(
                Matrix& covariance_matrix,
                state::StateVector& mean_vector);
            void cholesky(Matrix& L);

            void setRows(int rows);
            void setCols(int cols);

            int getRows();
            int getCols();
            int getSize();

        private:
            double* matrix_;
            int rows_;
            int cols_;

    };

    typedef Matrix DataMatrix;
    typedef Matrix CovarianceMatrix;

}


#endif

