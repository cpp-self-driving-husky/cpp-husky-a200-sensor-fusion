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


void testMatrixA() {

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
    covariance_matrix.cholesky(cholesky_matrix);

    printMatrix("Original Matrix",data_matrix);
    printVector("Mean Vector",mean_vector);
    printMatrix("Covariance Matrix",covariance_matrix);
    printMatrix("Cholesky Matrix",cholesky_matrix);

}



void testMatrixB() {

    int cols = 5;
    std::vector<double> state_samples = {5,2,8,1,6};
    state::StateVector<double> state(cols);
    for (int i = 0; i < cols; ++i)
        state[i] = state_samples[i];

    std::vector<double> matrix_samples = { 3,4,7,2,9,6,8,4,3,1,7,9,8,2,5 };
    int vars = static_cast<int>(matrix_samples.size());
    int rows = vars/cols;
    mtx::Matrix<double> matrix(rows,cols);
    matrix.init(rows,cols);
    for (int i = 0; i < vars; ++i)
        matrix[i] = matrix_samples[i];

    state::StateVector<double> copied_state(cols);
    state.replicate(copied_state);

    printVector("Original Vector",state);
    printVector("Copied Vector",copied_state);
    printMatrix("Original Matrix",matrix);
    matrix.addVectorMatrixRow(state,1);
    matrix.mean(copied_state);
    printVector("Added Vector",state);
    printVector("Copied Mean Vector",copied_state);
    matrix.subVectorMatrixRow(copied_state,0);
    printVector("Copied Mean Sub Vector",copied_state);

    mtx::Matrix<double> copied_matrix(rows,cols);
    matrix.replicate(copied_matrix);
    printMatrix("Copied Matrix",copied_matrix);

    matrix.transpose(copied_matrix);
    printMatrix("Copied Transpose Matrix",copied_matrix);

    state::StateVector<double> mean_state(copied_matrix.getCols());
    copied_matrix.mean(mean_state);
    printVector("Copied Mean Vector",mean_state);

    copied_matrix.init(matrix.getRows(),matrix.getCols());
    printMatrix("Reinitialized Matrix",copied_matrix);

}


void testInverseA() {

    int N = 5;

    state::StateVector<int> permutation(N);
    mtx::Matrix<double> matrix(N,N);
    mtx::Matrix<double> inverse(N,N);

    placeElementsA(matrix);

    for (int i = 0; i < matrix.getSize(); ++i)
        inverse[i] = matrix[i];

    inverse.LUPdecompose(permutation);
    matrix.print();

    state::StateVector<double> X(N);
    state::StateVector<double> Y(N);
    mtx::Matrix<double> intermediate(N,N);

    inverse.LUPinverse(intermediate,permutation,X,Y);

    inverse.print();

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


void testCalculatorDecomposeA() {

    int dim = 5;
    calc::Calculator<double> calc(dim);
    mtx::Matrix<double> matrix(dim,dim);
    state::StateVector<int> permutation(dim);
    placeElementsA(matrix);

    matrix.print();
    calc.LUPdecompose(matrix);
    matrix.print();

    //matrix.print();
    //matrix.LUPdecompose(permutation);
    //matrix.print();

}



int main(int argc, char* argv[]) {

    auto start = std::chrono::system_clock::now();

    //testCalculatorDecomposeA();
    testCalculatorF();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "\n" << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s" << "\n\n";

    return 0;
}




