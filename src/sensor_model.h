#include "sigma_points.h"


namespace sensor {

    template<class T>
    class SensorModel {

        public:
            SensorModel() {

            }

            virtual ~SensorModel() {

            }

            virtual void calculate(
                sigma::SigmaPoints<T>& observation,
                sigma::SigmaPoints<T>& prediction) = 0;


        private:

    };


    template<class T>
    class SimpleSensorModel : public SensorModel<T> {

        public:
            SimpleSensorModel() {

            }

            virtual ~SimpleSensorModel() {

            }

            virtual void calculate(
                sigma::SigmaPoints<T>& observation,
                sigma::SigmaPoints<T>& prediction)
            {
                int points = prediction.getNumPoints();
                for (int i = 0; i < points; ++i) {
                    observation[i][0] = prediction[i][0];
                    observation[i][1] = prediction[i][1];
                }

                //int points = prediction.getNumPoints(),
                //    vars = prediction.getStateSize();
                //for (int i = 0; i < points; ++i)
                //    for (int j = 0; j < vars; ++j)
                //        observation[i][j] = 0.0;


            }


    };

};

