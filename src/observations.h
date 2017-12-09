#ifndef OBSERVATION_H_
#define OBSERVATION_H_


namespace obs {

    class Observation {

        public:
            Observation();
            ~Observation();

            // TODO make this private
            void create();
            // TOOD make this private

            void create(double* data, int meas, int vars);
            void clear();

            double operator[](int i);

            void addData(double* data, int meas);
            void removeData(int meas);


            // TODO remove these getter functions!
            //      for debugging only!
            double* getData() {
                return this->data_;
            }
            int getMeas() {
                return this->meas_;
            }
            int getVars() {
                return this->vars_;
            }
            // TODO remove these getter functions!


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

#endif // OBSERVATION_H_
