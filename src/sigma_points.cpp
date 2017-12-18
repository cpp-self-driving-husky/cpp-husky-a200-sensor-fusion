#include <cmath>
#include "sigma_points.h"
#include "state_vector.h"
#include "matrix.h"


sigma::SigmaPoints::SigmaPoints(int state_size) :
    points_(nullptr), state_size_(state_size), elements_(0)
{
    this->init(state_size);
}


sigma::SigmaPoints::~SigmaPoints() {
    this->destroy();
}


void sigma::SigmaPoints::init(int state_size) {
    this->destroy();
    this->state_size_ = state_size;
    this->elements_ = this->pointsPerState(this->state_size_);
    this->points_ = new state::StateVector[this->elements_];
    for (int i = 0; i < this->elements_; ++i)
        this->points_[i].init(this->state_size_);
        //this->points_[i] = state::StateVector(this->state_size_);
}


void sigma::SigmaPoints::destroy() {
    if (this->points_ != nullptr) {
        delete[] this->points_;
        this->points_ = nullptr;
    }
    this->state_size_ = this->elements_ = 0;
}


// perhaps calculate cholesky outside of this function?
void sigma::SigmaPoints::generatePoints(
    state::StateVector& state,
    mtx::CovarianceMatrix& covariance_matrix,
    mtx::Matrix& cholesky_matrix,
    double gamma)
{
    covariance_matrix.cholesky(cholesky_matrix);
    cholesky_matrix *= gamma;
    int vars = state.getVars();
    for (int n = 0; n < this->elements_; ++n)
        for (int i = 0; i < vars; ++i)
            this->points_[n][i] = state[i];
    int offset = 1;
    for (int n = offset; n < this->state_size_+1; ++n)
        this->points_[n].addVectorMatrixRow(cholesky_matrix,n-offset);
    offset = this->state_size_+1;
    for (int n = offset; n < this->elements_; ++n)
        this->points_[n].subVectorMatrixRow(cholesky_matrix,n-offset);
}


double sigma::SigmaPoints::calculateLambda(
    double alpha, double kappa, int n)
{
    return std::pow(alpha,2)*(n+kappa)-n;
}


double sigma::SigmaPoints::calculateGamma(double lambda, int n) {
    return std::sqrt(n+lambda);
}


int sigma::SigmaPoints::pointsPerState(int state_size) {
    return state_size*2+1;
}


state::StateVector& sigma::SigmaPoints::operator[](int i) {
    return this->points_[i];
}


int sigma::SigmaPoints::getSize() {
    return this->elements_;
}




// end of file

