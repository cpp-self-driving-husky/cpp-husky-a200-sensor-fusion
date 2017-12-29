/*


#include <cmath>
#include "weight_vector.h"


weight::WeightVector::WeightVector(int n) :
    mean_val_(0.0), cov_val_(0.0), ith_val_(0.0), vars_(2*n+1)
{}


weight::WeightVector::~WeightVector() {

}


double weight::WeightVector::generateLambda(
    double alpha, double kappa, int n)
{
    return std::pow(alpha,2) * (n+kappa) - n;
}


double weight::WeightVector::zeroMeanCalc(
    double lambda, int state_size)
{
    return lambda / (state_size + lambda);
}


double weight::WeightVector::zeroCovCalc(
    double lambda, int state_size,
    double alpha, double beta)
{
    return
        (lambda / (state_size+lambda)) +
        (1 - std::pow(alpha,2) + beta);
}


double weight::WeightVector::ithMeanCovCalc(
    double lambda,
    int state_size)
{
    return lambda / (2 * (state_size + lambda));
}


void weight::WeightVector::calculateWeight(
    double lambda, int n,
    double alpha, double beta)
{
    this->mean_val_ = this->zeroMeanCalc(lambda,n);
    this->cov_val_ = this->zeroCovCalc(lambda,n,alpha,beta);
    this->ith_val_ = this->ithMeanCovCalc(lambda,n);
}


int weight::WeightVector::getVars() {
    return this->vars_;
}


double& weight::WeightVector::ithMean(int i) {
    if (i == 0)
        return this->mean_val_;
    else
        return this->ith_val_;
}


double& weight::WeightVector::ithCovariance(int i) {
    if (i == 0)
        return this->cov_val_;
    else
        return this->ith_val_;
}


// end of file


*/

