#include "state_vector.h"
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



// addData assumes only measurements with same number
// of variables will be added each time
// would have to clear old data to add new data
// to change number of variables
void mtx::Matrix::addData(double* data, int meas) {

    int orig_size = this->cols_*this->rows_;
    int new_size = this->cols_*(this->rows_+meas);
    double* temp = new double[new_size];
    for (int i = 0; i < orig_size; ++i)
        temp[i] = this->matrix_[i];
    for (int i = orig_size; i < new_size; ++i)
        temp[i] = data[i-orig_size];
    if (this->matrix_ != nullptr)
        delete[] this->matrix_;
    this->matrix_ = temp;
    this->rows_ += meas;

}


// TODO repupose the function so that for meas
//      a positive number removes older values
//      a negative number removes newer values
void mtx::Matrix::removeData(int meas) {

    int orig_size = this->cols_*this->rows_;
    // TODO check if number of measures to remove
    //      is greater than measures in data
    int new_size = this->cols_*(this->rows_-meas);
    int index = orig_size - new_size;
    double* temp = new double[new_size];
    for (int i = index; i < orig_size; ++i)
        temp[i-index] = this->matrix_[i];
    // this condition is probably not necessary
    if (this->matrix_ != nullptr)
        delete[] this->matrix_;
    this->matrix_ = temp;
    this->rows_ -= meas;

}


void mtx::Matrix::operator*=(double scalar) {
    for (int r = 0; r < this->rows_; ++r)
        for (int c = 0; c < this->cols_; ++c)
            this->matrix_[r*this->cols_+c] *= scalar;
}


void mtx::Matrix::setRows(int rows) {
    this->rows_ = rows;
}


void mtx::Matrix::setCols(int cols) {
    this->cols_ = cols;
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


void mtx::MatrixCalculator::transpose(
    mtx::Matrix& A, mtx::Matrix& T)
{
    int rows = A.getRows();
    int cols = A.getCols();
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            T[j*rows+i] = A[i*cols+j];
    T.setRows(cols);
    T.setCols(rows);
}


void mtx::MatrixCalculator::covariance(
    mtx::CovarianceMatrix& covariance_matrix,
    state::StateVector& mean_vector,
    mtx::DataMatrix& data_matrix)
{
    int meas = data_matrix.getRows();
    int vars = data_matrix.getCols();
    int dimension = covariance_matrix.getRows();
    for (int i = 0; i < dimension; ++i) {
        for (int j = i; j < dimension; ++j) {
            double y_mean = mean_vector[i];
            double x_mean = mean_vector[j];
            double variance = 0.0;
            for (int k = 0; k < meas; ++k)
                variance +=
                    (data_matrix[k*vars+j]-x_mean) *
                    (data_matrix[k*vars+i]-y_mean);
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



