#ifndef MEAN_VECTOR_H_
#define MEAN_VECTOR_H_

namespace cov {
	class CovarianceMatrix;
}


namespace vec {

	struct MeanVector {

		MeanVector(int size);
		MeanVector(double* elements, int size);
		~MeanVector();

		void init(int size);
		void clear();
		
		// assumes row major order
		void calculate_mean_vector(
			double* elements,
			int rows, int cols);
		//cov::CovarianceMatrix create_covariance_matrix();

		double operator[](int i);


		// temporary debugging fxn
		void print_mean_vector();


		double* elements_;
		int size_;

	};

}


#endif // !MEAN_VECTOR_H_

