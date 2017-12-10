#ifndef STATE_VECTOR_H_
#define STATE_VECTOR_H_


namespace data {
    class DataMatrix;
}


namespace state {

    class StateVector {

        public:
            StateVector();
            ~StateVector();

            void init(int vars);
            void init(double* vec, int vars);
            double& operator[](int i);
            int getVars();

        protected:
            double* vec_;
            int vars_;

    };

    typedef StateVector ControlVector;
    typedef StateVector MeasurementVector;


    class MeanVector : public StateVector {

        public:
            MeanVector();
            ~MeanVector();

            void calculate(data::DataMatrix& data_matrix);

        private:


    };

}





#endif // STATE_VECTOR_H_

