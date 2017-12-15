#include "state_vector.h"
#include "matrix.h"
#include <iostream>


state::StateVector::StateVector() :
    vec_(nullptr), vars_(0)
{}


state::StateVector::~StateVector() {
    if (this->vec_ != nullptr) {
        delete[] this->vec_;
        this->vec_ = nullptr;
    }
    this->vars_ = 0;
}


void state::StateVector::init(int vars) {
    this->vars_ = vars;
    if (this->vec_ != nullptr)
        delete[] this->vec_;
    this->vec_ = new double[this->vars_];
    for (int i = 0; i < this->vars_; ++i)
        this->vec_[i] = 0.0;
}


void state::StateVector::init(double* vec, int vars) {
    this->vars_ = vars;
    if (this->vec_ != nullptr)
        delete[] this->vec_;
    this->vec_ = new double[this->vars_];
    for (int i = 0 ; i < this->vars_; ++i)
        this->vec_[i] = vec[i];
}


double& state::StateVector::operator[](int i) {
    return this->vec_[i];
}


int state::StateVector::getVars() {
    return this->vars_;
}


// TODO perhaps check if this->vars_ == data_matrix.getVars
void state::StateVector::mean(mtx::DataMatrix& data_matrix) {
    int meas = data_matrix.getRows();
    for (int v = 0; v < this->vars_; ++v) {
        this->vec_[v] = 0.0;
        for (int m = 0; m < meas; ++m) {
            this->vec_[v] += data_matrix[this->vars_*m+v];
        }
        this->vec_[v] /= meas;
    }
}
