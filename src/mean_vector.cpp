#include "mean_vector.h"
#include "covariance_matrix.h"
#include <iostream>


vec::MeanVector::MeanVector(int size) :
	elements_(nullptr), size_(size)
{
	this->init(size);
}


vec::MeanVector::MeanVector(double* elements, int size) :
	elements_(nullptr), size_(size)
{
	this->init(size);
	for (int i = 0; i < size; ++i)
		this->elements_[i] = elements[i];
}


vec::MeanVector::~MeanVector() {
	this->clear();
}



void vec::MeanVector::init(int size) {
	this->clear();
	this->elements_ = new double[size];
	for (int i = 0; i < size; ++i)
		this->elements_[i] = 0.0;
}


void vec::MeanVector::clear() {
	if (this->elements_ != nullptr) {
		delete[] this->elements_;
		this->elements_ = nullptr;
	}
}


void vec::MeanVector::calculate_mean_vector(double* elements, int rows, int cols) {
	this->init(cols);
	for (int c = 0; c < cols; ++c) {
		for (int r = 0; r < rows; ++r) {
			elements_[c] += elements[cols*r+c];
		}
		elements_[c] /= rows;
	}
}




double vec::MeanVector::operator[](int i) {
	return this->elements_[i];
}


void vec::MeanVector::print_mean_vector() {
	for (int i = 0; i < size_; ++i)
		std::cout << elements_[i] << " ";
	std::cout << std::endl;
}




