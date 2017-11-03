#ifndef MEAN_VECTOR_H_
#define MEAN_VECTOR_H_


namespace vec {

	struct MeanVector {

		MeanVector() {

		}

		MeanVector(double* data, int rows, int cols) {
			this->calculateFromData(data,rows,cols);
		}

		~MeanVector() {
			if (this->elements_ != nullptr) {
				delete[] this->elements_;
				this->elements_ = nullptr;
			}
		}

		double* initDoublePtr(int cols) {
			double* ptr = new double[size];
			for (int i = 0; i < size; ++i)
				ptr[i] = 0.0;
			return ptr;
		}

		void calculateFromData(double* data, int rows, int cols) {
			this->elements_ = this->initDoublePtr(cols);
			for (int c = 0; c < cols; ++c) {
				for (int r = 0; r < rows; ++r) {
					this->elements_[c] += data[cols*r + c];
				}
				this->elements_[c] /= rows;
			}
		}

		MeanVector operator+(MeanVector& rh);

		double* elements_;

	};

}


#endif // !MEAN_VECTOR_H_
