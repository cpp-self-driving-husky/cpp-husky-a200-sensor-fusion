#include "state_vector.h"
#include "matrix.h"
#include <iostream>


state::StateVector::StateVector(int vars) :
    vec_(nullptr), vars_(vars)
{
    this->init(vars);
}


state::StateVector::~StateVector() {
    this->destroy();
}


void state::StateVector::init(int vars) {
    this->destroy();
    this->vars_ = vars;
    this->vec_ = new double[this->vars_];
    for (int i = 0; i < this->vars_; ++i)
        this->vec_[i] = 0.0;
}


// TODO check to make sure other state is proper size
void state::StateVector::replicate(state::StateVector& state) {
    //state.init(this->vars_);
    for (int i = 0; i < this->vars_; ++i)
        state[i] = this->vec_[i];
}


void state::StateVector::destroy() {
    if (this->vec_ != nullptr) {
        delete[] this->vec_;
        this->vec_ = nullptr;
    }
    this->vars_ = 0;
}

// TODO perhaps check if this->vars_ == data_matrix.getVars
void state::StateVector::mean(mtx::DataMatrix& data_matrix) {
    int meas = data_matrix.getRows();
    for (int v = 0; v < this->vars_; ++v) {
        this->vec_[v] = 0.0;
        for (int m = 0; m < meas; ++m)
            this->vec_[v] += data_matrix[this->vars_*m+v];
        this->vec_[v] /= meas;
    }
}


void state::StateVector::addVectorMatrixRow(
    mtx::Matrix& matrix,
    int init_row)
{
    int cols = matrix.getCols();
    int init_elem = init_row * cols;
    for (int i = 0; i < cols; ++i)
        this->vec_[i] += matrix[init_elem+i];
}


void state::StateVector::subVectorMatrixRow(
    mtx::Matrix& matrix,
    int init_row)
{
    int cols = matrix.getCols();
    int init_elem = init_row * cols;
    for (int i = 0; i < cols; ++i)
        this->vec_[i] -= matrix[init_elem+i];
}


double& state::StateVector::operator[](int i) {
    return this->vec_[i];
}


int state::StateVector::getVars() {
    return this->vars_;
}



/*

state::VectorCalculator::VectorCalculator() {

}


state::VectorCalculator::~VectorCalculator() {

}


void state::VectorCalculator::addVectorMatrixRow(
    state::StateVector& state,
    mtx::Matrix& matrix,
    int init_row)
{
    int cols = matrix.getCols();
    int init_elem = init_row * cols;
    for (int i = 0; i < cols; ++i)
        state[i] += matrix[init_elem+i];
}


void state::VectorCalculator::subVectorMatrixRow(
    state::StateVector& state,
    mtx::Matrix& matrix,
    int init_row)
{
    int cols = matrix.getCols();
    int init_elem = init_row * cols;
    for (int i = 0; i < cols; ++i)
        state[i] -= matrix[init_elem+i];
}


*/


// end of file


