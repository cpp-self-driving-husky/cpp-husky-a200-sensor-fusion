#ifndef SENSOR_MODEL_H_
#define SENSOR_MODEL_H_
#include "models.h"


namespace model {


    template<class T>
    class SimpleSensorVelocityModel : public SensorModel<T> {

        public:
            SimpleSensorVelocityModel() :
                SensorModel<T>()
            {}

            ~SimpleSensorVelocityModel() {

            }

            virtual void calculate(
                sigma::SigmaPoints<T>& observation,
                sigma::SigmaPoints<T>& prediction)
            {
                int points = prediction.rows();
                for (int i = 0; i < points; ++i) {
                    observation(i,0) = prediction(i,0);
                    observation(i,1) = prediction(i,1);
                }
            }

        private:

    };


};


#endif

