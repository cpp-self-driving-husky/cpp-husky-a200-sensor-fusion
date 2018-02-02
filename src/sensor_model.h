#include "sigma_points.h"


namespace sensor {

    template<class T>
    class SensorModel {

        public:
            SensorModel() {

            }

            virtual ~SensorModel() {

            }

            virtual void calculate(sigma::SigmaPoints<T>& sigma) = 0;


        private:

    };


    template<class T>
    class SimpleSensorModel : public SensorModel<T> {

        public:
            SimpleSensorModel() {

            }

            virtual ~SimpleSensorModel() {

            }

            virtual void calculate(sigma::SigmaPoints<T>& sigma) {



            }


    };

};

