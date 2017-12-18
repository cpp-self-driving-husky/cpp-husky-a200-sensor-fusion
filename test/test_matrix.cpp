#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "../src/state_vector.h"
#include "../src/matrix.h"



void print_vector(
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


void print_matrix(std::string msg, mtx::Matrix& matrix) {
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
    mtx::Matrix data_matrix(side,side);
    for (int i = 0; i < elems; ++i)
        data_matrix[i] = covariance_samples[i];

    state::StateVector mean_vector(side);
    mean_vector.mean(data_matrix);
    mtx::Matrix covariance_matrix(side,side);
    data_matrix.covariance(covariance_matrix,mean_vector);

    mtx::Matrix cholesky_matrix(side,side);
    covariance_matrix.cholesky(cholesky_matrix);

    print_matrix("Original Matrix",data_matrix);
    print_matrix("Covariance Matrix",covariance_matrix);
    print_matrix("Cholesky Matrix",cholesky_matrix);

}


void testMatrixB() {

    int cols = 5;
    std::vector<double> state_samples = {5,2,8,1,6};
    state::StateVector state(cols);
    for (int i = 0; i < cols; ++i)
        state[i] = state_samples[i];

    std::vector<double> matrix_samples = { 3,4,7,2,9,6,8,4,3,1,7,9,8,2,5 };
    int vars = static_cast<int>(matrix_samples.size());
    int rows = vars/cols;
    mtx::Matrix matrix(rows,cols);
    matrix.init(rows,cols);
    for (int i = 0; i < vars; ++i)
        matrix[i] = matrix_samples[i];

    state::StateVector copied_state(cols);
    state.replicate(copied_state);

    print_vector("Original Vector",state);
    print_vector("Copied Vector",copied_state);
    print_matrix("Original Matrix",matrix);
    state.addVectorMatrixRow(matrix,1);
    copied_state.mean(matrix);
    print_vector("Added Vector",state);
    print_vector("Copied Mean Vector",copied_state);
    copied_state.subVectorMatrixRow(matrix,0);
    print_vector("Copied Mean Sub Vector",copied_state);

    mtx::Matrix copied_matrix(rows,cols);
    matrix.replicate(copied_matrix);
    print_matrix("Copied Matrix",copied_matrix);

    matrix.transpose(copied_matrix);
    print_matrix("Copied Transpose Matrix",copied_matrix);

    state::StateVector mean_state(copied_matrix.getCols());
    mean_state.mean(copied_matrix);
    print_vector("Copied Mean Vector",mean_state);

    copied_matrix.init(matrix.getRows(),matrix.getCols());
    print_matrix("Reinitialized Matrix",copied_matrix);

}



int main(int argc, char* argv[]) {
    testMatrixA();
    return 0;
}




// end of file

