#include <iostream>
#include <typeinfo>
#include <cstdlib>
#include <ctime>
#include "test.h"



void test::Test::print_vector(
    std::string msg,
    state::StateVector& xt)
{
    std::cout << "\n" << msg << "\n";
    int vars = xt.getVars();
    for (int i = 0; i < vars; ++i)
        std::cout << xt[i] << " ";
    std::cout << "\n" <<
        "variables " << vars << "\n\n";

}


void test::Test::print_matrix(std::string msg, mtx::Matrix& matrix) {
    std::cout << msg << "\n";
    for (int r = 0; r < matrix.getRows(); ++r) {
        for (int c = 0; c < matrix.getCols(); ++c) {
            std::cout << matrix[r*matrix.getCols()+c] << " ";
        }
        std::cout << "\n";
    }
    std::cout <<
        "rows: " << matrix.getRows() << ", " <<
        "cols: " << matrix.getCols() << "\n";
    std::cout << std::endl;
}




void test::Test::testMatrixA() {

    std::cout << "\n" << "Matrix Function Tests" << "\n\n";

    int rows = 3, cols = 3;
    mtx::Matrix matrix;
    matrix.init(rows,cols);

    for (int i = 0; i < matrix.getRows()*matrix.getCols(); ++i)
        matrix[i] = i;
    double elem = matrix.elem(1,2);

    int vars = rows;
    state::StateVector state;
    state.init(vars);
    for (int i = 0; i < vars; ++i)
        state[i] = 100+i;

    this->print_matrix("Matrix Test A",matrix);

    this->print_vector("Vector Test A",state);
    matrix.stateCol(state,1);
    this->print_vector("Vector Test B",state);
    matrix.stateRow(state,1);
    this->print_vector("Vector Test C",state);

    double* sample = new double[rows*cols];

    sample[0] = 2;
    sample[1] = 3;
    sample[2] = 4;
    sample[3] = 0;
    sample[4] = 4;
    sample[5] = 3;
    sample[6] = -9;
    sample[7] = 1;
    sample[8] = -5;

    mtx::DataMatrix data_matrix;
    //data_matrix.init(sample,rows,cols);

    data_matrix.init(rows,cols);
    for (int i = 0; i < rows*cols; ++i)
        data_matrix[i] = sample[i];

    delete[] sample;
    this->print_matrix("Print Data Matrix",data_matrix);

    mtx::Matrix covariance_matrix;
    covariance_matrix.init(rows,cols);
    for (int i = 0; i < rows*cols; ++i)
        covariance_matrix[i] = i;
    this->print_matrix(
        "Print Covariance Pre Calculation",
        covariance_matrix);

    state::StateVector mean_vector;
    mean_vector.init(rows);
    mean_vector.mean(data_matrix);
    this->print_vector("Print Mean Vector",mean_vector);

    mtx::MatrixCalculator calculator;
    calculator.covariance(
        covariance_matrix,
        mean_vector,
        data_matrix);

    this->print_matrix(
        "Print Covariance After Calculation",
        covariance_matrix);

    covariance_matrix[0] = 25;
    covariance_matrix[1] = 15;
    covariance_matrix[2] = -5;
    covariance_matrix[3] = 15;
    covariance_matrix[4] = 18;
    covariance_matrix[5] = 0;
    covariance_matrix[6] = -5;
    covariance_matrix[7] = 0;
    covariance_matrix[8] = 11;

    mtx::Matrix cholesky_matrix;
    cholesky_matrix.init(rows,cols);
    cholesky_matrix[0] = 18;
    cholesky_matrix[1] = 22;
    cholesky_matrix[2] = 54;
    cholesky_matrix[3] = 22;
    cholesky_matrix[4] = 70;
    cholesky_matrix[5] = 86;
    cholesky_matrix[6] = 54;
    cholesky_matrix[7] = 86;
    cholesky_matrix[8] = 174;

    calculator.cholesky(cholesky_matrix,covariance_matrix);
    this->print_matrix(
        "Print Cholesky Matrix",
        cholesky_matrix);

    double factor = 3.0;
    this->print_matrix("Print Original Matrix",matrix);
    matrix *= factor;
    this->print_matrix("Print Multiplied Matrix",matrix);



}


void test::Test::testMatrixB() {

    int rows = 3, cols = 3;
    mtx::Matrix matrix;
    matrix.init(rows,cols);

    matrix[0] = 8;
    matrix[1] = 2;
    matrix[2] = 1;
    matrix[3] = 5;
    matrix[4] = 6;
    matrix[5] = 2;
    matrix[6] = 9;
    matrix[7] = 0;
    matrix[8] = 5;
    //matrix[9] = 3;
    //matrix[10] = 7;
    //matrix[11] = 4;

    //print_matrix("Original Matrix",matrix);
    //matrix.transpose();
    //print_matrix("Transposed Matrix",matrix);

}



void test::Test::testMotionModelA() {

    srand(2);

    int vars = 3;
    state::StateVector xt_f;
    state::StateVector xt_i;
    state::StateVector ut_f;
    state::StateVector ut_i;
    xt_f.init(vars);
    xt_i.init(vars);
    ut_f.init(vars);
    ut_i.init(vars);

    for (int i = 0; i < vars; ++i) {
        xt_f[i] = rand()/1000000000.0;
        xt_i[i] = rand()/1000000000.0;
        ut_f[i] = rand()/1000000000.0;
        ut_i[i] = rand()/1000000000.0;
    }

    std::string msgx = "Test Vector X";
    this->print_vector(msgx,xt_f);
    this->print_vector(msgx,xt_i);

    std::string msgu = "Test Vector U";
    this->print_vector(msgu,ut_f);
    this->print_vector(msgu,ut_i);


    model::OdometryMotionModel odometry;
    double res = odometry.calculate(xt_f,xt_i,ut_f,ut_i);

    std::cout << "\n" <<
        "Odometry: p(xt|ut,xt-1) = " << res << "\n" << std::endl;


}


void test::Test::testMatrixTranspose() {

    mtx::MatrixCalculator calculator;

    int rows = 3, cols = 4;
    mtx::Matrix A;
    A.init(rows,cols);
    A[0] = 7;
    A[1] = 2;
    A[2] = 6;
    A[3] = 9;
    A[4] = 1;
    A[5] = 7;
    A[6] = 8;
    A[7] = 4;
    A[8] = 3;
    A[9] = 7;
    A[10] = 5;
    A[11] = 9;

    mtx::Matrix T;
    T.init(3,4);

    this->print_matrix("Pre Trans A",A);
    this->print_matrix("Pre Trans T",T);
    calculator.transpose(A,T);
    this->print_matrix("Post Trans A",A);
    this->print_matrix("Post Trans T",T);


}


void test::Test::testAddVectorMatrix() {

    state::VectorCalculator calculator;

    int rows = 3, cols = 5;
    state::StateVector state;
    state.init(cols);
    state[0] = 5;
    state[1] = 4;
    state[2] = 9;
    state[3] = 2;
    state[4] = 5;

    mtx::Matrix matrix;
    matrix.init(rows,cols);
    matrix[0] = 7;
    matrix[1] = 2;
    matrix[2] = 9;
    matrix[3] = 4;
    matrix[4] = 1;
    matrix[5] = 6;
    matrix[6] = 8;
    matrix[7] = 4;
    matrix[8] = 2;
    matrix[9] = 7;
    matrix[10] = 8;
    matrix[11] = 1;
    matrix[12] = 0;
    matrix[13] = 3;
    matrix[14] = 5;

    this->print_matrix("Matrix",matrix);
    this->print_vector("Vector",state);

    int added_row = 0;
    calculator.addVectorMatrixRow(state,matrix,added_row);

    this->print_vector("Sum Vector",state);


}




/*

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
    state::MeanVector mean_vector;
    mean_vector.init(vars);
    mean_vector.calculate(data_matrix);
    this->print_vector(msg_mean,mean_vector);


    msg_data = "Test Data Matrix B";
    meas = 4;
    samples = new double[vars*meas];
    for (int i = 0; i < meas*vars; ++i)
        samples[i] = 100+i;


    data_matrix.addData(samples,meas);
    delete[] samples;
    this->print_matrix(msg_data,data_matrix);


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

*/

/*

void test::Test::testMeanVectorB(int seed) {

    srand(seed);
    //std::cout << rand() << std::endl;

    int meas = 10, vars = 5;
    double* samples = new double[meas*vars];
    for (int i = 0; i < meas*vars; ++i)
        samples[i] = ((rand() % 1000000) / 10000.0 );

    data::DataMatrix data;
    data.init(samples,meas,vars);
    delete[] samples;

    std::string msg_data = "Test Data Matrix A";
    this->print_matrix(msg_data,data);

    std::string msg_vec = "Test Means Vector A";
    state::MeanVector vec;
    vec.init(vars);
    vec.calculate(data);
    this->print_vector(msg_vec,vec);

}

*/




/*

void test::Test::testCovarianceMatrix() {

    int meas = 5, vars = 3;
    double* samples = new double[meas*vars];
    samples[0] = 4.0;
    samples[1] = 2.0;
    samples[2] = 0.6;
    samples[3] = 4.2;
    samples[4] = 2.1;
    samples[5] = 0.59;
    samples[6] = 3.9;
    samples[7] = 2.0;
    samples[8] = 0.58;
    samples[9] = 4.3;
    samples[10] = 2.1;
    samples[11] = 0.62;
    samples[12] = 4.1;
    samples[13] = 2.2;
    samples[14] = 0.63;

    data::DataMatrix data_matrix;
    data_matrix.init(samples,meas,vars);
    delete[] samples;

    std::string msg_data = "Test Data Matrix";
    this->print_matrix(msg_data,data_matrix);

    std::string msg_vec = "Test Mean Vector";
    state::MeanVector mean_vector;
    mean_vector.init(vars);
    mean_vector.calculate(data_matrix);
    this->print_vector(msg_vec,mean_vector);

    std::string msg_cov = "Test Covariance Matrix";
    cov::CovarianceMatrix covariance;
    covariance.init(vars);
    covariance.calculate(data_matrix,mean_vector);
    this->print_matrix(msg_cov,covariance);


}

*/


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

