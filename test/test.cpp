#include <iostream>
#include <typeinfo>
#include "test.h"


void test::Test::print_matrix(
    std::string& message, double* matrix,
    int rows, int cols)
{
    std::cout << "\n" << message << "\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            std::cout << matrix[r*cols+c] << " ";
        }
        std::cout << "\n";
    }
    std::cout <<
        "rows: " << rows << ", " <<
        "cols: " << cols << "\n";
    std::cout << std::endl;
}


void test::Test::print_matrix(
    std::string& message,
    data::DataMatrix& matrix)
{
    this->print_matrix(
        message,
        matrix.getData(),
        matrix.getMeas(),
        matrix.getVars());
}


void test::Test::print_vector(
    std::string& message,
    vec::MeanVector& vec)
{
    std::cout << "\n" << message << "\n";
    int vars = vec.getVars();
    for (int i = 0; i < vars; ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";
    std::cout << "variables " << vars << "\n";
    std::cout << "\n" << std::endl;
}


void test::Test::testMeanVectorA() {

    data::DataMatrix data_matrix;
    int meas = 5, vars = 3;
    double* samples = new double[meas*vars];
    for (int i = 0; i < meas*vars; ++i) {
        samples[i] = i;
    }
    data_matrix.init(samples,meas,vars);
    delete[] samples;

    std::string msg_data = "Test Data Matrix A";
    this->print_matrix(
        msg_data,data_matrix.getData(),
        data_matrix.getMeas(),
        data_matrix.getVars());


    std::string msg_mean = "Test Mean Vector A";
    vec::MeanVector mean_vector;
    mean_vector.calculate(data_matrix);
    this->print_vector(msg_mean,mean_vector);

    msg_data = "Test Data Matrix B";
    meas = 4;
    samples = new double[vars*meas];
    for (int i = 0; i < meas*vars; ++i)
        samples[i] = 100+i;
    data_matrix.addData(samples,meas);
    delete[] samples;
    this->print_matrix(
        msg_data,data_matrix.getData(),
        data_matrix.getMeas(),data_matrix.getVars());

    msg_mean = "Test Mean Vector B";
    mean_vector.calculate(data_matrix);
    this->print_vector(msg_mean,mean_vector);


    msg_data = "Test Data Matrix C";
    data_matrix.removeData(3);
    this->print_matrix(msg_data,data_matrix);

    msg_mean = "Test Mean Vector C";
    mean_vector.calculate(data_matrix);
    this->print_vector(msg_mean,mean_vector);

}




/*
void test::Test::print_vector(
    std::string& message,
    vec::MeanVector& vec)
{
    std::cout << "\n" << message << "\n";
    int vars = vec.getVars();
    for (int i = 0; i < vars; ++i)
        std::cout << vec[i] << " ";
    std::cout << "\n" <<
        "variables: " << vars <<
        "\n" << std::endl;
}
*/


/*

data::DataMatrix test::Test::testDataMatrixA(bool output) {

    // testing data creation
    std::string create_message = "Test Data Creation";
    int meas = 5, vars = 3;
    double* raw = new double[meas*vars];
    for (int i = 0; i < meas*vars; ++i)
        raw[i] = i;
    data::DataMatrix data_matrix;
    data_matrix.init(raw,meas,vars);
    if (output) {
        this->print_matrix(
            create_message,data_matrix.getData(),
            data_matrix.getMeas(),data_matrix.getVars());
    }
    delete[] raw;

    // testing data addition
    std::string addition_message = "Test Data Addition";
    int prev_vars = vars, prev_meas = meas;
    vars = 3, meas = 2;
    raw = new double[meas*vars];
    for (int i = 0; i < meas*vars; ++i)
        raw[i] = 100+i;
    data_matrix.addData(raw,meas);
    if (output) {
        this->print_matrix(
            addition_message,data_matrix.getData(),
            data_matrix.getMeas(),data_matrix.getVars());
    }
    delete[] raw;

    // testing data removal
    std::string removal_message = "Test Data Removal";
    data_matrix.removeData(3);
    if (output) {
        this->print_matrix(
            removal_message,data_matrix.getData(),
            data_matrix.getMeas(),data_matrix.getVars());
    }

    return data_matrix;

}

*/


/*

vec::MeanVector test::Test::testMeanVectorA() {

    //vec::MeanVector mean_vector;
    //mean_vector.init(data);
    //std::string data_message = "Data Matrix";
    //this->print_matrix(data_message,data);

    //std::string message = "Test Mean Vector";
    //this->print_vector(message,mean_vector);


    data::DataMatrix data_matrix = data::DataMatrix();

    int meas = 5, vars = 3;
    double* samples = new double[meas*vars];
    for (int i = 0; i < meas*vars; ++i) {
        samples[i] = i;
    }
    data_matrix.init(samples,meas,vars);
    delete[] samples;

    std::string msg = "Test data matrix";
    this->print_matrix(
        msg,data_matrix.getData(),
        data_matrix.getMeas(),
        data_matrix.getVars());


    //vec::MeanVector mean_vec;
    //std::cout << " hey" << std::endl;

}

*/


/*

vec::MeanVector test::Test::testMeanVectorA(data::DataMatrix& data) {

    //vec::MeanVector mean_vector();
    vec::MeanVector mean_vector;
    mean_vector.init(data);
    std::string data_message = "Data Matrix";
    this->print_matrix(data_message,data);

    std::string message = "Test Mean Vector";
    this->print_vector(message,mean_vector);

}

*/


/*

#include <iostream>
#include "../src/covariance_matrix.h"
#include "test.h"


void test::Test::testCovarianceMatrix() {

	int rows = 3, cols = 5;
	double* samples = new double[rows*cols];
	samples[0] = 4.0;
	samples[1] = 7.0;
	samples[2] = 9.0;
	samples[3] = 2.0;
	samples[4] = 1.0;
	samples[5] = 5.0;
	samples[6] = 5.0;
	samples[7] = 6.0;
	samples[8] = 8.0;
	samples[9] = 1.0;
	samples[10] = 0.0;
	samples[11] = 2.0;
	samples[12] = 3.0;
	samples[13] = 6.0;
	samples[14] = 8.0;

	cov::CovarianceMatrix covariance_matrix;
	double* covariance = covariance_matrix.calculateCovarianceMatrix(samples, 3, 5);

	// do some sort of comparison here
	for (int i = 0; i < cols*cols; ++i) {
		std::cout << covariance[i] << " ";
		if (i % cols == cols - 1) {
			std::cout << "\n";
		}
	}
	std::cout << std::endl;

}

*/

