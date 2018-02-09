#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <cmath>
//#include "../src/matrix.h"
#include "../src/calculator.h"



void printVector(
    std::string msg,
    state::StateVector<double>& xt)
{
    std::cout << "\n" << msg << "\n";
    int vars = xt.getVars();
    for (int i = 0; i < vars; ++i)
        std::cout << xt[i] << " ";
    std::cout << "\n" <<
        "variables " << vars << "\n\n";

}


void printMatrix(std::string msg, mtx::Matrix<double>& matrix) {
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


void placeElementsA(mtx::Matrix<double>& matrix) {

    std::vector<double> samples = {
        10,4,21,7,13,
        23,1,6,9,17,
        3,11,5,4,31,
        5,19,24,8,1,
        15,29,11,9,14
    };

    for (int i = 0; i < matrix.getSize(); ++i)
        matrix[i] = samples[i];

}


void placeCovarianceB(mtx::Matrix<double>& matrix) {

    std::vector<double> samples = {
        10,4,21,7,13,
        11,2,25,9,17,
        3,11,21,4,31,
        14,2,24,8,11,
        15,9,23,9,14
    };

    for (int i = 0; i < matrix.getSize(); ++i)
        matrix[i] = samples[i];

}


void placeElementsB(mtx::Matrix<double>& matrix) {

    std::vector<double> samples = {
        7,4,1,0,
        2,1,6,3,
        3,7,5,8
    };

    for (int i = 0; i < matrix.getSize(); ++i)
        matrix[i] = samples[i];

}


void placeElementsC(mtx::Matrix<double>& matrix) {

    std::vector<double> samples = {
        1,2,7,2,
        4,7,6,5,
        6,2,3,0
    };

    for (int i = 0; i < matrix.getSize(); ++i)
        matrix[i] = samples[i];

}


void placeCovarianceA(mtx::Matrix<double>& matrix) {

    std::vector<double> samples = {
        1,          0.307,      0.338,       0.302,
        0.307,      1,          0.642,       0.410,
        0.338,      0.642,      1,           0.360,
        0.302,      0.410,      0.360,       1
    };

    for (int i = 0; i < matrix.getSize(); ++i)
        matrix[i] = samples[i];

}


void testCalculatorStorage() {

    int row = 4, col = 6;
    calc::Calculator<double> calc(100);
    mtx::Matrix<double> a(row,col);
    placeElementsA(a);
    a.print();

    auto start = std::chrono::system_clock::now();

    calc.transpose(a);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    a.print();
    std::cout << "\n" << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s" << "\n\n";

}


void testCalculatorA() {

    int row = 3, col = 4;
    mtx::Matrix<double> A(row,col);
    mtx::Matrix<double> B(col,row);
    mtx::Matrix<double> P(row,row);
    placeElementsB(A);
    placeElementsC(B);

    calc::Calculator<double> calculator(col);
    calculator.multiply(P,A,B);

    A.print();
    B.print();
    P.print();

}


void testCalculatorB() {

    int obs = 4, vars = 5;
    state::StateVector<double> state(vars);
    mtx::Matrix<double> data(obs,vars);
    mtx::Matrix<double> matrix(vars,vars);
    mtx::Matrix<double> cholesky(vars,vars);
    placeCovarianceB(data);

    data.covariance(matrix,state);

    calc::Calculator<double> calc(vars);
    calc.cholesky(cholesky,matrix);

    matrix.print();
    cholesky.print();

}


void testCalculatorC() {

    std::vector<double> covariance_samples = {
        10,4,21,7,13,
        23,1,6,9,17,
        3,11,5,4,31,
        5,19,24,8,8,
        15,29,11,9,14
    };
    int side = 5;
    int elems = side*side;
    mtx::Matrix<double> data_matrix(side,side);
    for (int i = 0; i < elems; ++i)
        data_matrix[i] = covariance_samples[i];

    state::StateVector<double> mean_vector(side);
    data_matrix.mean(mean_vector);
    mtx::Matrix<double> covariance_matrix(side,side);
    data_matrix.covariance(covariance_matrix,mean_vector);

    mtx::Matrix<double> cholesky_matrix(side,side);

    calc::Calculator<double> calc(side);
    calc.cholesky(cholesky_matrix,covariance_matrix);

    covariance_matrix.print();
    cholesky_matrix.print();

}


void testCalculatorD() {

    int rows = 6, cols = 3;
    mtx::Matrix<double> matrix(rows,cols);
    mtx::Matrix<double> transpose(rows,cols);
    placeElementsA(matrix);
    calc::Calculator<double> calc(rows);
    calc.transpose(transpose,matrix);
    matrix.print();
    transpose.print();

}


void testCalculatorE() {

    int rows = 6, cols = 3;
    mtx::Matrix<double> transpose(rows,cols);
    placeElementsA(transpose);
    calc::Calculator<double> calc(rows);
    transpose.print();
    calc.transpose(transpose);
    transpose.print();

}


void testCalculatorF() {

    int side = 5;
    //int elems = side*side;

    calc::Calculator<double> calc;
    mtx::Matrix<double> data_matrix(side,side);
    mtx::Matrix<double> covariance(side,side);
    mtx::Matrix<double> cholesky(side,side);

    state::StateVector<double> mean_vector(side);
    placeElementsA(data_matrix);

    calc.init(side);

    data_matrix.covariance(covariance,mean_vector);
    covariance.print();

    calc.cholesky(cholesky,covariance);
    cholesky.print();

    calc.cholesky(covariance);
    covariance.print();



}


void testCalculatorInverseA() {

    int dim = 5;
    calc::Calculator<double> calc(dim);

    mtx::Matrix<double> a(dim,dim);
    mtx::Matrix<double> b(dim,dim);
    mtx::Matrix<double> c(dim,dim);
    placeElementsA(a);
    c = a;

    calc.inverse(b,a);
    calc.inverse(c);

    a.print();
    b.print();
    c.print();

}



int main(int argc, char* argv[]) {

    auto start = std::chrono::system_clock::now();

    testCalculatorInverseA();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "\n" << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s" << "\n\n";

    return 0;
}




