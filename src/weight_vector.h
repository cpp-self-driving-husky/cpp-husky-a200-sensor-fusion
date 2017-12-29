/*


#ifndef WEIGHT_VECTOR_H_
#define WEIGHT_VECTOR_H_


namespace weight {

    class WeightVector {

        public:
            WeightVector(int vars);
            ~WeightVector();

            double generateLambda(
                double alpha, double kappa, int n);

            double zeroMeanCalc(
                double lambda, int state_size);
            double zeroCovCalc(
                double lambda, int state_size,
                double alpha, double beta);
            double ithMeanCovCalc(
                double lambda, int state_size);

            void calculateWeight(
                double lambda, int state_size,
                double alpha, double beta);

            int getVars();
            double& ithMean(int i);
            double& ithCovariance(int i);

        private:
            double mean_val_;
            double cov_val_;
            double ith_val_;
            int vars_;

    };

}


#endif // WEIGHT_VECTOR_H_


*/
