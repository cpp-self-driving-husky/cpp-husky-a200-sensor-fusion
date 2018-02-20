#ifndef MATRIX_H_
#define MATRIX_H_
#include <cmath>
#include "state_vector.h"


// TODO remove inclusion
//      test for printing
#include <iostream>
#include <iomanip>


namespace mtx {

    template<class T>
    class Matrix {

        public:
            Matrix(const int rows, const int cols) :
                matrix_(nullptr),rows_(rows),cols_(cols)
            {
                this->init(rows,cols);
            }

            Matrix(const Matrix<T>& M) :
                matrix_(nullptr),rows_(0),cols_(0)
             {
                this->init(M.getRows(),M.getCols());
                for (int i = 0; i < M.getSize(); ++i)
                    this->matrix_[i] = M.matrix_[i];
            }

            ~Matrix() {
                this->destroy();
            }

            void init(const int rows, const int cols) {
                this->destroy();
                this->rows_ = rows;
                this->cols_ = cols;
                int elem = this->rows_ * this->cols_;
                this->matrix_ = new T[elem];
                this->zero();
            }

            void duplicate(const Matrix<T>& matrix) {
                for (int i = 0; i < this->getSize(); ++i)
                    this->matrix_[i] = matrix[i];
            }

            //void replicate(Matrix<T>& matrix) {
            //    int elems = this->rows_*this->cols_;
            //    for (int i = 0; i < elems; ++i)
            //        matrix[i] = this->matrix_[i];
            //}

            void zero() {
                int elems = this->rows_*this->cols_;
                for (int i = 0; i < elems; ++i)
                    this->matrix_[i] = 0.0;
            }

            void destroy() {
                if (this->matrix_ != nullptr) {
                    delete[] this->matrix_;
                    this->matrix_ = nullptr;
                }
                this->rows_ = this->cols_ = 0;
            }

            T& elem(int row, int col) {
                return this->matrix_[row*this->cols_+col];
            }

            T& operator[](int i) {
                return this->matrix_[i];
            }

            mtx::Matrix<T>& operator=(mtx::Matrix<T>& M) {
                int len = M.getSize();
                for (int i = 0; i < len; ++i)
                    this->matrix_[i] = M[i];
                return *this;
            }

            void operator+=(mtx::Matrix<T>& matrix) {
                int elems = this->getSize();
                for (int i = 0; i < elems; ++i)
                    this->matrix_[i] += matrix[i];
            }

            void operator-=(mtx::Matrix<T>& matrix) {
                int elems = this->getSize();
                for (int i = 0; i < elems; ++i)
                    this->matrix_[i] -= matrix[i];
            }

            void operator*=(T scalar) {
                for (int r = 0; r < this->rows_; ++r)
                    for (int c = 0; c < this->cols_; ++c)
                        this->matrix_[r*this->cols_+c] *= scalar;
            }

            void addData(T* data, int meas) {
                int orig_size = this->cols_*this->rows_;
                int new_size = this->cols_*(this->rows_+meas);
                T* temp = new T[new_size];
                for (int i = 0; i < orig_size; ++i)
                    temp[i] = this->matrix_[i];
                for (int i = orig_size; i < new_size; ++i)
                    temp[i] = data[i-orig_size];
                if (this->matrix_ != nullptr)
                    delete[] this->matrix_;
                this->matrix_ = temp;
                this->rows_ += meas;
            }

            void removeData(int meas) {
                int orig_size = this->cols_*this->rows_;
                int new_size = this->cols_*(this->rows_-meas);
                int index = orig_size - new_size;
                T* temp = new T[new_size];
                for (int i = index; i < orig_size; ++i)
                    temp[i-index] = this->matrix_[i];
                if (this->matrix_ != nullptr)
                    delete[] this->matrix_;
                this->matrix_ = temp;
                this->rows_ -= meas;
            }

            void swapDimensions() {
                int sub = this->rows_;
                this->rows_ = this->cols_;
                this->cols_ = sub;
            }

            void setRows(int rows) {
                this->rows_ = rows;
            }

            void setCols(int cols) {
                this->cols_ = cols;
            }

            const int getRows() const {
                return this->rows_;
            }

            const int getCols() const {
                return this->cols_;
            }

            const int getSize() const {
                return this->rows_ * this->cols_;
            }

            void precisionPrint() {
                int def_prec = 6,
                    prec = 17;
                std::cout << std::setprecision(prec);
                this->print();
                std::cout << std::setprecision(def_prec);
            }

            // test function
            void print() {
                for (int r = 0; r < this->rows_; ++r) {
                    for (int c = 0; c < this->cols_; ++c) {
                        std::cout << this->matrix_[r*this->cols_+c] << " ";
                    }
                    std::cout << "\n";
                }
                std::cout <<
                    "rows: " << this->rows_ << ", " <<
                    "cols: " << this->cols_ << "\n";
                std::cout << std::endl;
            }


        private:
            T* matrix_;
            int rows_;
            int cols_;

    };

    // a data matrix places observations as rows,
    // and each variable is in columns
    template<class T>
    using DataMatrix = Matrix<T>;

    template<class T>
    using CovarianceMatrix = Matrix<T>;

}


#endif

