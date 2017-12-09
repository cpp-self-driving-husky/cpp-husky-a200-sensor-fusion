#include "mean_vector.h"
#include "data_matrix.h"


vec::MeanVector::MeanVector() :
    vec_(nullptr), vars_(0)
{}


vec::MeanVector::~MeanVector() {
    if (this->vec_ != nullptr) {
        delete[] this->vec_;
        this->vec_ = nullptr;
    }
}


double vec::MeanVector::operator[](int i) {
    return this->vec_[i];
}


void vec::MeanVector::calculate(data::DataMatrix& data_matrix) {

    this->vars_ = data_matrix.getVars();
    int meas = data_matrix.getMeas();
    if (this->vec_ != nullptr) {
        delete[] this->vec_;
        this->vec_ = nullptr;
    }
    this->vec_ = new double[this->vars_];
    for (int v = 0; v < this->vars_; ++v) {
        this->vec_[v] = 0.0;
        for (int m = 0; m < meas; ++m) {
            this->vec_[v] += data_matrix[this->vars_*m+v];
        }
        this->vec_[v] /= meas;
    }

}


int vec::MeanVector::getVars() {
    return this->vars_;
}

