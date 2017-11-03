#ifndef COVARIANCE_H_
#define COVARIANCE_H_


namespace vec {
	struct MeanVector;
}


namespace cov {

	class CovarianceMatrix {

	public:
		CovarianceMatrix();
		CovarianceMatrix(int size);
		~CovarianceMatrix();

		double* initDoublePtr(int size);
		double* calculateMeanVector(double* data, int rows, int cols);


		void init(
			double*vector,
			int vector_size);
		void clear();

		double calculateMean(double* samples, int size);
		double sampleCovariance(
			double* x_s, double* y_s,
			int sample_size);
		
		//void calculateCovarianceMatrix(vec::MeanVector& mean_vector);

		double* calculateCovarianceMatrix(double* data, int rows, int cols);


	private:
		double* matrix_;
		double* vector_;
		int vector_size_;


	};


}


#endif // !COVARIANCE_H_
