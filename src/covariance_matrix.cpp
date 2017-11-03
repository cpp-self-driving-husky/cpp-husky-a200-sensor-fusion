#include <iostream>
#include "covariance_matrix.h"


cov::CovarianceMatrix::CovarianceMatrix() {

}


cov::CovarianceMatrix::~CovarianceMatrix() {

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
	delete[] mean_vector;
	return covariance_matrix;
}



