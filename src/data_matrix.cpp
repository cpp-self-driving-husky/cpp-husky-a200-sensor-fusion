#include "data_matrix.h"
#include <iostream>


data::DataMatrix::DataMatrix() :
    data_(nullptr), meas_(0), vars_(0)
{}


data::DataMatrix::~DataMatrix() {
    if (this->data_ != nullptr) {
        delete[] this->data_;
        this->data_ = nullptr;
    }
}


void data::DataMatrix::init(double* data, int meas, int vars) {
    this->vars_ = vars;
    this->meas_ = 0;
    this->addData(data,meas);
}


double data::DataMatrix::operator[](int i) {
    return this->data_[i];
}


// addData assumes only measurements with same number
// of variables will be added each time
// would have to clear old data to add new data
// to change number of variables
void data::DataMatrix::addData(double* data, int meas) {

    int orig_size = this->vars_*this->meas_;
    int new_size = this->vars_*(this->meas_+meas);
    double* temp = new double[new_size];
    for (int i = 0; i < orig_size; ++i)
        temp[i] = this->data_[i];
    for (int i = orig_size; i < new_size; ++i)
        temp[i] = data[i-orig_size];
    if (this->data_ != nullptr)
        delete[] this->data_;
    this->data_ = temp;
    this->meas_ += meas;

}


// TODO repupose the function so that for meas
//      a positive number removes older values
//      a negative number removes newer values
void data::DataMatrix::removeData(int meas) {

    int orig_size = this->vars_*this->meas_;
    // TODO check if number of measures to remove
    //      is greater than measures in data
    int new_size = this->vars_*(this->meas_-meas);
    int index = orig_size - new_size;
    double* temp = new double[new_size];
    for (int i = index; i < orig_size; ++i)
        temp[i-index] = this->data_[i];
    // this condition is probably not necessary
    if (this->data_ != nullptr)
        delete[] this->data_;
    this->data_ = temp;
    this->meas_ -= meas;

}


int data::DataMatrix::getMeas() {
    return this->meas_;
}


int data::DataMatrix::getVars() {
    return this->vars_;
}


