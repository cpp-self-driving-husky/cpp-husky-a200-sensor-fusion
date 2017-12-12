#include "state_vector.h"
#include "data_matrix.h"
#include "matrix.h"
#include <cmath>


mtx::Matrix::Matrix() :
    matrix_(nullptr),
    rows_(0), cols_(0)
{}


mtx::Matrix::~Matrix() {
    if (this->matrix_ != nullptr) {
        delete[] this->matrix_;
        this->matrix_ = nullptr;
    }
    rows_ = cols_ = 0;
}


void mtx::Matrix::init(int rows, int cols) {
    if (this->matrix_ != nullptr)
        delete[] this->matrix_;
    this->rows_ = rows;
    this->cols_ = cols;
    int elem = this->rows_ * this->cols_;
    this->matrix_ = new double[elem];
    for (int i = 0; i < elem; ++i)
        this->matrix_[i] = 0.0;
}


double mtx::Matrix::elem(int row, int col) {
    return this->matrix_[row*this->cols_+col];
}


double& mtx::Matrix::operator[](int i) {
    return this->matrix_[i];
}


// TODO check to make sure state and size matrix are same
void mtx::Matrix::stateCol(state::StateVector& state, int col) {
    for (int i = 0; i < this->rows_; ++i)
        state[i] = this->matrix_[i*this->rows_+col];
}


void mtx::Matrix::stateRow(state::StateVector& state, int row) {
    int init_elem = this->cols_*row;
    for (int i = 0; i < this->cols_; ++i)
        state[i] = this->matrix_[i+init_elem];
}


int mtx::Matrix::getRows() {
    return this->rows_;
}


int mtx::Matrix::getCols() {
    return this->cols_;
}


int mtx::Matrix::getSize() {
    return this->rows_ * this->cols_;
}


// Matrix Calculator class


mtx::MatrixCalculator::MatrixCalculator() {

}


mtx::MatrixCalculator::~MatrixCalculator() {

}


void mtx::MatrixCalculator::covariance(
    mtx::Matrix& covariance_matrix,
    state::StateVector& mean_vector,
    data::DataMatrix& data_matrix)
{
    int meas = data_matrix.getMeas();
    int vars = data_matrix.getVars();
    int dimension = covariance_matrix.getRows();
    for (int i = 0; i < dimension; ++i) {
        for (int j = i; j < dimension; ++j) {
            double y_mean = mean_vector[i];
            double x_mean = mean_vector[j];
            double variance = 0.0;
            for (int k = 0; k < meas; ++k) {
                double y_s = data_matrix[k*vars+i];
                double x_s = data_matrix[k*vars+j];
                variance += (x_s-x_mean)*(y_s-y_mean);
            }
            variance /= (meas-1);
            covariance_matrix[i*dimension+j] = variance;
            covariance_matrix[j*dimension+i] = variance;
        }
    }
}


void mtx::MatrixCalculator::cholesky(
    mtx::Matrix& L, mtx::Matrix& A)
{
    int n = A.getRows();
    int elems = L.getRows()*L.getCols();
    for (int i = 0; i < elems; ++i)
        L[i] = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (i+1); ++j) {
            double s = 0;
            for (int k = 0; k < j; k++)
                s += L[i*n+k] * L[j*n+k];
            L[i*n+j] = (i == j) ?
                std::sqrt(A[i*n+i] - s) :
                (1.0 / L[j*n+j] * (A[i*n+j] - s));
        }
    }
}


//void mtx::operator*=(double scalar, mtx::Matrix& matrix) {
//    int rows = matrix.rows_;
//    int cols = matrix.cols_;
//    for (int r = 0; r < rows; ++r) {
//        for (int c = 0; c < cols; ++c) {
//            matrix[r*cols+c] *= scalar;
//        }
//    }
//}




/*
void cov::CovarianceMatrix::calculate(
    data::DataMatrix& data_matrix,
    vec::MeanVector& mean_vector)
{
    int meas = data_matrix.getMeas();
    int vars = data_matrix.getVars();
    for (int i = 0; i < this->vars_; ++i) {
        for (int j = i; j < this->vars_; ++j) {
            double y_mean = mean_vector[i];
            double x_mean = mean_vector[j];
            double variance = 0.0;
            for (int k = 0; k < meas; ++k) {
                double y_s = data_matrix[k*vars+i];
                double x_s = data_matrix[k*vars+j];
                variance += (x_s-x_mean)*(y_s-y_mean);
            }
            variance /= (meas-1);
            this->matrix_[i*this->vars_+j] = variance;
            this->matrix_[j*this->vars_+i] = variance;
        }
    }
}
*/



