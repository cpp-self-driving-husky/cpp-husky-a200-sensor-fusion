/*

#ifndef DATA_MATRIX_H_
#define DATA_MATRIX_H_


namespace data {

    class DataMatrix {

        public:
            DataMatrix();
            ~DataMatrix();

            void init(double* data, int meas, int vars);

            double operator[](int i);

            void addData(double* data, int meas);
            void removeData(int meas);


            // TODO remove this getter function...
            //      for debugging only!
            //double* getData() {
            //    return this->data_;
            //}


            int getMeas();
            int getVars();


        private:
            // TODO potentially change data such that
            //      memory does not have to be allocated
            //      each time, but has a pointer which
            //      cyclically loops around on a fixed
            //      number of rows used
            double* data_;
            int meas_;      // rows
            int vars_;      // cols

    };

}

#endif // DATA_MATRIX_H_

*/
