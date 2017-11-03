#ifndef COVARIANCE_H_
#define COVARIANCE_H_


namespace cov {

	class CovarianceMatrix {

	public:
		CovarianceMatrix();
		~CovarianceMatrix();

		double* initDoublePtr(int size);
		double* calculateMeanVector(double* data, int rows, int cols);
		double* calculateCovarianceMatrix(double* data, int rows, int cols);


	private:
		

	};


}


#endif // !COVARIANCE_H_
