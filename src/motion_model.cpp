/*

#include "motion_model.h"
#include <cmath>


// TODO remove, used for debugging
#include <iostream>


namespace {

    // TODO these are empirically found values
    //      and will be modified after their
    //      empirical values are tested
    const double alpha_1 = 1.0;
    const double alpha_2 = 1.0;
    const double alpha_3 = 1.0;
    const double alpha_4 = 1.0;

}




// Motion Model Parent Class

model::MotionModel::MotionModel() {

}


model::MotionModel::~MotionModel() {

}


double model::MotionModel::distance(
    double x_f, double x_i,
    double y_f, double y_i)
{
    return std::sqrt(std::pow(x_f-x_i,2)+std::pow(y_f-y_i,2));
}


double model::MotionModel::variance(
    double a1, double d1, double a2, double d2)
{
    return
        a1*std::pow(d1,2)+
        a2*std::pow(d2,2);
}


double model::MotionModel::variance(
    double a1, double d1, double a2, double d2, double d3)
{
    return
        a1*std::pow(d1,2)+
        a2*std::pow(d2,2)+
        a2*std::pow(d3,2);
}

// Motion Model Parent Class




// Motion Model Child Classes

model::OdometryMotionModel::OdometryMotionModel() :
    MotionModel(),
    delta_i_(state::ParameterVector(0)),
    delta_f_(state::ParameterVector(0))
{
    int def_vars = 3;
    this->delta_i_.init(def_vars);
    this->delta_f_.init(def_vars);
}


model::OdometryMotionModel::~OdometryMotionModel() {

}


double model::OdometryMotionModel::init_rotation(
    state::ControlVector& x_tf,
    state::ControlVector& x_ti)
{
    double y = x_tf[POSE::Y] - x_ti[POSE::Y];
    double x = x_tf[POSE::X] - x_ti[POSE::X];
    double theta = x_ti[POSE::T];
    return atan2(y,x) - theta;
}


double model::OdometryMotionModel::translation(
    state::StateVector& x_tf,
    state::StateVector& x_ti)
{
    this->distance(
        x_tf[POSE::X],x_ti[POSE::X],
        x_tf[POSE::Y],x_ti[POSE::Y]);
}


double model::OdometryMotionModel::final_rotation(
    state::StateVector& x_tf,
    state::StateVector& x_ti,
    double rotation_1)
{
    return x_tf[POSE::T] - x_ti[POSE::T] - rotation_1;
}


double model::OdometryMotionModel::probability(
    double a, double b)
{
    double norm = 1.0/std::sqrt(2*M_PI*b);
    double expon = std::exp(-std::pow(a,2)/(2*b));
    return norm*expon;
}


double model::OdometryMotionModel::calculate(
    state::StateVector& x_tf, state::StateVector& x_ti,
    state::ControlVector& u_tf, state::ControlVector& u_ti)
{
    this->delta_i_[MOTION::ROT_1] = this->init_rotation(u_tf,u_ti);
    this->delta_i_[MOTION::TRANS] = this->translation(u_tf,u_ti);
    this->delta_i_[MOTION::ROT_2] = this->final_rotation(
        u_tf,u_ti,this->delta_i_[MOTION::ROT_1]);

    this->delta_f_[MOTION::ROT_1] = this->init_rotation(x_tf,x_ti);
    this->delta_f_[MOTION::TRANS] = this->translation(x_tf,x_ti);
    this->delta_f_[MOTION::ROT_2] = this->final_rotation(
        x_tf,x_ti,this->delta_f_[MOTION::ROT_1]);

    double p1 = this->probability(
        this->delta_i_[MOTION::ROT_1]-this->delta_f_[MOTION::ROT_1],
        this->variance(
            alpha_1,this->delta_f_[MOTION::ROT_1],
            alpha_2,this->delta_f_[MOTION::TRANS]));

    double p2 = this->probability(
        this->delta_i_[MOTION::TRANS]-this->delta_f_[MOTION::TRANS],
        this->variance(
            alpha_3,this->delta_f_[MOTION::TRANS],
            alpha_4,this->delta_f_[MOTION::ROT_1],
            this->delta_f_[MOTION::ROT_2]));

    double p3 = this->probability(
        this->delta_i_[MOTION::ROT_2]-this->delta_f_[MOTION::ROT_2],
        this->variance(
            alpha_1,this->delta_f_[MOTION::ROT_2],
            alpha_2,this->delta_f_[MOTION::TRANS]));

    return p1*p2*p3;

}

*/


