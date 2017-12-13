
/*

#ifndef COVARIANCE_MATRIX_H_
#define COVARIANCE_MATRIX_H_


namespace data {
    class DataMatrix;
}

namespace vec {
    class MeanVector;
}


namespace cov {

    class CovarianceMatrix {

        public:
            CovarianceMatrix();
            ~CovarianceMatrix();

            void init(int vars);
            void calculate(
                data::DataMatrix& data_matrix,
                vec::MeanVector& mean_vector);

            double operator[](int i);

            int getVars();

        private:
            double* matrix_;
            int vars_;

    };

}


#endif // COVARIANCE_MATRIX_H_

*/
