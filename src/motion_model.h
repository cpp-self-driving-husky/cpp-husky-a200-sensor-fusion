#ifndef MOTION_MODEL_H_
#define MOTION_MODEL_H_
#include "state_vector.h"


namespace model {

    class MotionModel {

        public:
            MotionModel();
            virtual ~MotionModel();

        protected:
            virtual double calculate(
                state::StateVector& x_tf,
                state::StateVector& x_ti,
                state::ControlVector& u_tf,
                state::ControlVector& u_ti) = 0;

            virtual double probability(
                double a, double b) = 0;

            double distance(
                double x_f, double x_i,
                double y_f, double y_i);

            double variance(
                double a1, double d1,
                double a2, double d2);

            double variance(
                double a1, double d1,
                double a2, double d2,
                double d3);


        private:


    };


    class OdometryMotionModel : public MotionModel {

        public:
            OdometryMotionModel();
            ~OdometryMotionModel();

            virtual double calculate(
                state::StateVector& x_tf,
                state::StateVector& x_ti,
                state::ControlVector& u_tf,
                state::ControlVector& u_ti);


        private:
            double init_rotation(
                state::StateVector& x_tf,
                state::StateVector& x_ti);

            double translation(
                state::StateVector& x_tf,
                state::StateVector& x_ti);

            double final_rotation(
                state::StateVector& x_tf,
                state::StateVector& x_ti,
                double rotation_1);

            virtual double probability(
                double a, double b);

            enum MOTION {
                ROT_1 = 0,
                TRANS = 1,
                ROT_2 = 2
            };

            enum POSE {
                X = 0,
                Y = 1,
                T = 2
            };

            state::ParameterVector delta_i_;
            state::ParameterVector delta_f_;

    };

}


#endif // MOTION_MODEL_H_


