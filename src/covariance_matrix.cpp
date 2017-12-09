#include "covariance_matrix.h"
#include "mean_vector.h"
#include "data_matrix.h"


cov::CovarianceMatrix::CovarianceMatrix() :
    matrix_(nullptr), vars_(0)
{}


cov::CovarianceMatrix::~CovarianceMatrix() {
    if (this->matrix_ != nullptr) {
        delete[] this->matrix_;
        this->matrix_ = nullptr;
    }
}


void cov::CovarianceMatrix::init(int vars) {
    this->vars_ = vars;
    if (this->matrix_ != nullptr)
        delete[] this->matrix_;
    this->matrix_ = new double[this->vars_*this->vars_];
}


void cov::CovarianceMatrix::calculate(
    data::DataMatrix& data_matrix,
    vec::MeanVector& mean_vector)
{
    int meas = data_matrix.getMeas();
    // TODO ensure data_matrix vars is same
    //      as covariant matrix this->vars_
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


double cov::CovarianceMatrix::operator[](int i) {
    return this->matrix_[i];
}


int cov::CovarianceMatrix::getVars() {
    return this->vars_;
}


