#ifndef CALCULATOR_H_
#define CALCULATOR_H_
#include "matrix.h"
#include <cmath>


// TODO remove inclusion
//      test for printing
#include<iostream>


namespace calc {

    template<class T>
    class Calculator {

        public:
            Calculator(int vars, int meas) :
                trans_store_(mtx::Matrix<T>(vars,meas)),
                inv_store_(mtx::Matrix<T>(meas,meas)),
                state_x_(nullptr),
                state_y_(nullptr),
                indeces_(nullptr),
                vars_(0),
                meas_(0)
            {
                this->init(vars,meas);
            }

            ~Calculator() {
                this->destroy();
            }

            void init(int vars,int meas) {
                this->destroy();
                this->vars_ = vars;
                this->meas_ = meas;
                this->trans_store_.init(vars,meas);
                this->inv_store_.init(meas,meas);
                this->state_x_ = new T[this->meas_];
                this->state_y_ = new T[this->meas_];
                this->indeces_ = new int[this->meas_];
            }

            void destroy() {
                this->trans_store_.destroy();
                this->inv_store_.destroy();
                this->deallocate(this->state_x_);
                this->deallocate(this->state_y_);
                this->deallocate(this->indeces_);
                this->vars_ = 0;
            }

            template<typename S>
            void deallocate(S* elements) {
                if (elements != nullptr) {
                    delete[] elements;
                    elements = nullptr;
                }
            }

            template<typename S>
            void zero(S* elements, int len) {
                for (int i = 0; i < len; ++i)
                    elements[i] = 0.0;
            }

            template<typename S>
            void swaps(S& a, S& b) {
                S s = a;
                a = b;
                b = s;
            }

            void mean(state::StateVector<T>& state, mtx::Matrix<T>& data) {
                int vars = state.getVars();
                int obser = data.getRows();
                for (int v = 0; v < vars; ++v) {
                    state[v] = 0.0;
                    for (int m = 0; m < obser; ++m)
                        state[v] += data[vars*m+v];
                    state[v] /= obser;
                }
            }

            void covariance(
                mtx::Matrix<T>& cov,
                mtx::Matrix<T>& data,
                state::StateVector<T>& mean)
            {
                int obser = data.getRows(),
                    vars = data.getCols();
                for (int i = 0; i < vars; ++i) {
                    for (int j = i; j < vars; ++j) {
                        T variance = 0.0;
                        for (int k = 0; k < obser; ++k) {
                            variance +=
                                (data[k*vars+i]-mean[i]) *
                                (data[k*vars+j]-mean[j]);
                        }
                        variance /= obser-1;
                        cov[i*vars+j] = variance;
                        cov[j*vars+i] = variance;
                    }
                }
            }

            void transpose(mtx::Matrix<T>& trans, mtx::Matrix<T>& matrix) {
                int rows = trans.getRows(),
                    cols = trans.getCols();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        trans[j*rows+i] = matrix[i*cols+j];
                trans.swapDimensions();
            }

            void subtract(mtx::Matrix<T>& P, mtx::Matrix<T>& A, mtx::Matrix<T>& B) {
                int len = P.getSize();
                for (int i = 0; i < len; ++i)
                    P[i] = A[i] - B[i];
            }

            void multiply(mtx::Matrix<T>& P,mtx::Matrix<T>& A,mtx::Matrix<T>& B) {
                P.zero();
                int rows = A.getRows(),
                    inner = A.getCols(),
                    cols = B.getCols();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        for (int k = 0; k < inner; ++k)
                            P[i*cols+j] += A[i*inner+k] * B[k*cols+j];
            }

            void multiplyABt(mtx::Matrix<T>& P,mtx::Matrix<T>& A,mtx::Matrix<T>& B) {
                P.zero();
                int rows = A.getRows(),
                    inner = A.getCols(),
                    cols = B.getRows();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        for (int k = 0; k < inner; ++k)
                            P[i*cols+j] += A[i*inner+k] * B[j*inner+k];
            }

            void multiplyABAt(mtx::Matrix<T>& P,mtx::Matrix<T>& A,mtx::Matrix<T>& B) {
                this->multiply(this->trans_store_,A,B);
                this->multiplyABt(P,this->trans_store_,A);
            }

            // TODO there is some rounding error
            //      which causes problems in calculation!
            void cholesky(mtx::Matrix<T>& L, mtx::Matrix<T>& A) {
                L.zero();
                int elems = L.getSize(),
                    n = L.getRows();
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < (i+1); ++j) {
                        T s = 0.0;
                        for (int k = 0; k < j; ++k)
                            s += L[i*n+k] * L[j*n+k];
                        L[i*n+j] = (i == j) ?
                            std::sqrt(A[i*n+i] - s) :
                            (1.0 / L[j*n+j] * (A[i*n+j] - s));
                    }
                }
            }

            bool LUPdecompose(mtx::Matrix<T>& L){
                int len = L.getRows(), kd = 0;
                for (int i = 0; i < len; ++i)
                    this->indeces_[i] = i;
                for (int k = 0; k < len-1; ++k) {
                    T p = 0.0;
                    for (int i = k; i < len; ++i) {
                        T t = L[i*len+k];
                        if (t < 0)
                            t *= -1;
                        if (t > p) {
                            p = t;
                            kd = i;
                        }
                    }
                    if (p == 0.0)
                        return false;
                    this->swaps(this->indeces_[kd],this->indeces_[k]);
                    for (int i = 0; i < len; ++i)
                        this->swaps(L[kd*len+i],L[k*len+i]);
                    for (int i = k+1; i < len; ++i) {
                        L[i*len+k] = L[i*len+k] / L[k*len+k];
                        for (int j = k+1; j < len; ++j)
                            L[i*len+j] -= L[i*len+k]*L[k*len+j];
                    }
                }
                return true;
            }

            bool LUPinverse(mtx::Matrix<T>& LU) {
                this->zero(this->state_x_,this->vars_);
                this->zero(this->state_y_,this->vars_);
                int len = LU.getRows();
                for (int i = 0; i < len; ++i) {
                    for (int j = 0; j < len; ++j)
                        this->inv_store_[i*len+j] = 0.0;
                    this->inv_store_[i*len+i] = 1.0;
                    for (int n = 0; n < len; ++n) {
                        T t = 0.0;
                        for (int m = 0; m <= n-1; ++m )
                            t += LU[n*len+m]*this->state_y_[m];
                        this->state_y_[n] = this->inv_store_[i*len+this->indeces_[n]]-t;
                    }
                    for (int n = len-1; n>=0; --n) {
                        T t = 0.0;
                        for (int m = n+1; m < len; ++m)
                            t += LU[n*len+m] * this->state_x_[m];
                        this->state_x_[n] = (this->state_y_[n]-t) / LU[n*len+n];
                    }
                    for (int j = 0; j < len; ++j)
                        this->inv_store_[i*len+j] = this->state_x_[j];
                }
                for (int i = 0; i < len; ++i)
                    for (int j = 0; j < len; ++j)
                        LU[i*len+j] = this->inv_store_[j*len+i];
                return true;
            }

            void inverse(mtx::Matrix<T>& I, mtx::Matrix<T>& M) {
                I = M;
                this->LUPdecompose(I);
                this->LUPinverse(I);
            }


        private:
            mtx::Matrix<T> trans_store_;
            mtx::Matrix<T> inv_store_;

            int* indeces_;
            T* state_x_;
            T* state_y_;

            int vars_;
            int meas_;

    };

}

    #endif

