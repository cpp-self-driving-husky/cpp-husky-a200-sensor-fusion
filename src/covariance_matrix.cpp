#include <iostream>
#include "covariance_matrix.h"


cov::CovarianceMatrix::CovarianceMatrix() :
    matrix_(nullptr)
 {}


cov::CovarianceMatrix::~CovarianceMatrix() {
    if (matrix_ != nullptr) {
        delete[] matrix_;
        matrix_ = nullptr;
    }
}











/*

#include <iostream>
#include "covariance_matrix.h"
#include "mean_vector.h"


cov::CovarianceMatrix::CovarianceMatrix() {

}


cov::CovarianceMatrix::CovarianceMatrix(int size) :
	//rows_(size), cols_(size),
	vector_size_(size)
{
	vector_ = new double[size];
	matrix_ = new double[size*size];
}


cov::CovarianceMatrix::~CovarianceMatrix() {
	this->clear();
}


double* cov::CovarianceMatrix::initDoublePtr(int size) {
	double* ptr = new double[size];
	for (int i = 0; i < size; ++i)
		ptr[i] = 0.0;
	return ptr;
}


double* cov::CovarianceMatrix::calculateMeanVector(double* data, int rows, int cols) {
	double* mean_vector = this->initDoublePtr(cols);
	for (int c = 0; c < cols; ++c) {
		for (int r = 0; r < rows; ++r) {
			mean_vector[c] += data[cols*r+c];
		}
		mean_vector[c] /= rows;
	}
	return mean_vector;
}


void cov::CovarianceMatrix::init(
	double* vector,
	int vector_size)
{
	this->clear();
	vector_size_ = vector_size;
	this->vector_ = new double[vector_size];
	for (int i = 0; i < vector_size; ++i)
		vector_[i] = vector[i];
	this->matrix_ = new double[vector_size*vector_size];
}


void cov::CovarianceMatrix::clear() {
	if (matrix_ != nullptr) {
		delete[] matrix_;
		matrix_ = nullptr;
	}
	if (vector_ != nullptr) {
		delete[] vector_;
		vector_ = nullptr;
	}
	vector_size_ = 0;
}


double cov::CovarianceMatrix::calculateMean(double* samples, int size) {
	double sum = 0;
	for (int i = 0; i < size; ++i)
		sum += samples[i];
	return sum / size;
}


// has been tested, works so far
double cov::CovarianceMatrix::sampleCovariance(
	double* x_s, double* y_s,
	int sample_size)
{
	double x_mean = this->calculateMean(x_s, sample_size),
		y_mean = this->calculateMean(y_s, sample_size);

	double covariance = 0.0;
	for (int i = 0; i < sample_size; ++i)
		covariance += (x_s[i] - x_mean)*(y_s[i] - y_mean);
	covariance /= sample_size-1;

	return covariance;
}


// TODO this needs some number of samples
//void cov::CovarianceMatrix::calculateCovarianceMatrix(vec::MeanVector& mean_vector) {
//	this->init(mean_vector.elements_, mean_vector.size_);
//	for (int i = 0; i < vector_size_; ++i) {
//		for (int j = 0; j < vector_size_; ++j) {
//			double elem_i = mean_vector[i];
//			double elem_j = mean_vector[j];
//
//		}
//	}
//
//}



// pass matrix of data
// each row is a vector of data
// each col is a datum
double* cov::CovarianceMatrix::calculateCovarianceMatrix(double* data, int rows, int cols) {
	double* mean_vector = this->calculateMeanVector(data,rows,cols);
	double* covariance_matrix = this->initDoublePtr(cols*cols);
	for (int c_a = 0; c_a < cols; ++c_a) {
		for (int c_b = 0; c_b < cols; ++c_b) {
			double mean_a = mean_vector[c_a],
				mean_b = mean_vector[c_b],
				covariance = 0.0;
			for (int r = 0; r < rows; ++r)
				covariance += (data[cols*r+c_a]-mean_a)*(data[cols*r+c_b]-mean_b);
			covariance /= rows-1;
			covariance_matrix[c_a*cols+c_b] = covariance_matrix[c_b*cols+c_a] = covariance;
		}
	}
	return covariance_matrix;
}

*/
