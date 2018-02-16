#include "utilitest.h"
#include "../src/calculator.h"



void testCovariance() {
    auto matrices = test::procesMatrices("data/covariance.txt");
    for (size_t i = 0; i < matrices.size(); ++i) {

        int cols = matrices[i].first.getCols();
        state::StateVector<double> mean(cols);
        mtx::Matrix<double> cov(cols,cols);
        calc::Calculator<double> calc(cols);

        calc.mean(mean,matrices[i].first);
        calc.covariance(cov,matrices[i].first,mean);

        test::run(cov,matrices[i].second);

    }
}





int main(int argc, char* argv[]) {


    testCovariance();

    return 0;

}
