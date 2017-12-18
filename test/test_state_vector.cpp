#include <iostream>
#include <string>
#include "../src/state_vector.h"
#include "../src/matrix.h"



void print_vector(
    std::string msg,
    state::StateVector& xt)
{
    std::cout << "\n" << msg << "\n";
    int vars = xt.getVars();
    for (int i = 0; i < vars; ++i)
        std::cout << xt[i] << " ";
    std::cout << "\n" <<
        "variables " << vars << "\n\n";
}



void testStateVectorB() {

    int vars = 7;
    state::StateVector state(vars);
    state.init(vars);

    state[0] = 5;
    state[1] = 9;
    state[2] = 2;
    state[3] = 7;
    state[4] = 4;
    state[5] = 1;
    state[6] = 8;


    print_vector("State Vector",state);

    state.destroy();
    print_vector("State Vector Clear",state);



}




/*

void testStateVectorA() {

    state::StateVector state_vector;

    int vars = 6;
    double* samples = new double[vars];
    samples[0] = 9;
    samples[1] = 7;
    samples[2] = 21;
    samples[3] = 15;
    samples[4] = 1;
    samples[5] = 17;

    state_vector.init(samples,vars);

    delete[] samples;

    std::cout << "State Vector Init" << "\n";
    for (int i = 0; i < state_vector.getVars(); ++i)
        std::cout << state_vector[i] << " ";
    std::cout << "\n\n";

    for (int i = 0; i < state_vector.getVars(); ++i)
        state_vector[i] = i;

    std::cout << "State Vector Index Operator" << "\n";
    for (int i = 0; i < state_vector.getVars(); ++i)
        std::cout << state_vector[i] << " ";
    std::cout << "\n\n";

    vars = 6;
    samples = new double[vars];
    samples[0] = 19;
    samples[1] = 21;
    samples[2] = 27;
    samples[3] = 5;
    samples[4] = 9;
    samples[5] = 13;


    state::ControlVector control_vector;
    control_vector.init(samples,vars);
    std::cout << "Control Vector Init" << "\n";
    for (int i = 0; i < control_vector.getVars(); ++i)
        std::cout << control_vector[i] << " ";
    std::cout << "\n\n";

    for (int i = 0; i < control_vector.getVars(); ++i)
        control_vector[i] += samples[i];

    delete[] samples;

    std::cout << "Control Vector Index Operator" << std::endl;
    for (int i = 0; i < control_vector.getVars(); ++i)
        std::cout << control_vector[i] << " ";
    std::cout << "\n" << std::endl;

}

*/


/*

void testMeanVectorA() {

    int meas = 5, vars = 3;
    double* samples = new double[meas*vars];
    samples[0] = 4.0;
    samples[1] = 2.0;
    samples[2] = 0.6;
    samples[3] = 4.2;
    samples[4] = 2.1;
    samples[5] = 0.59;
    samples[6] = 3.9;
    samples[7] = 2.0;
    samples[8] = 0.58;
    samples[9] = 4.3;
    samples[10] = 2.1;
    samples[11] = 0.62;
    samples[12] = 4.1;
    samples[13] = 2.2;
    samples[14] = 0.63;

    data::DataMatrix data_matrix;
    data_matrix.init(samples,meas,vars);
    delete[] samples;





    state::MeanVector mean_vec;


}

*/



int main(int argc, char* argv[]) {

    testStateVectorB();



    return 0;
}





