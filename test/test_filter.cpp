#include<iostream>
#include<string>
#include<vector>

#include "../src/ukf.h"


/*

void testMatrixMultiply() {

    std::vector<double> elems_a = {
        4,5,1,7,8,
        1,2,0,4,5,
        1,2,3,4,7
    };

    std::vector<double> elems_b = {
        8,7,2,
        4,5,6,
        2,7,2,
        3,4,5,
        9,1,5
    };

    int row_a = 3, col_a = 5;
    int row_b = 5, col_b = 3;
    int row_p = 3, col_p = 3;

    mtx::Matrix<double> product(row_p,col_p);
    mtx::Matrix<double> multiplier(row_a,col_a);
    mtx::Matrix<double> multiplicand(row_b,col_b);

    for (int i = 0; i < row_a*col_a; ++i) {
        multiplier[i] = elems_a[i];
        multiplicand[i] = elems_b[i];
    }

    ukf::UnscentedKalmanFilter filter(row_p);
    filter.multiplyMatrices(product,multiplier,multiplicand);

    product.print();

}

*/


void populateSigmaA(sigma::SigmaPoints<double>& sigma) {

    std::vector<double> elems = {
        8,7,2,
        4,5,6,
        2,7,2,
        3,4,5,
        9,1,5,
        5,2,6,
        4,7,1
    };
    size_t col = 3;

    for (int i = 0; i < sigma.getNumPoints(); ++i)
        for (int j = 0; j < sigma.getStateSize(); ++j)
            sigma[i][j] = elems[i*col+j];

}



void testWeightedCovarianceA() {


    int vars = 3;
    int points = 2*vars+1;
    ukf::UnscentedKalmanFilter<double> filter(vars);

    mtx::Matrix<double> covariance(vars,vars);
    mtx::Matrix<double> noise(vars,vars);
    state::WeightVector<double> weight(vars,points);
    state::StateVector<double> state(vars);
    sigma::SigmaPoints<double> sigma(points,vars);
    populateSigmaA(sigma);

    covariance.print();
    sigma.print();
    filter.sumWeightedCovariance(covariance,weight,sigma,state,noise);
    covariance.print();


}



void testFilterA() {



}



int main(int argc, char* argv[]) {
    testWeightedCovarianceA();
    return 0;
}



