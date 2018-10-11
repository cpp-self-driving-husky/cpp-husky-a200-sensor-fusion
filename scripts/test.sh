
cd ../

g++ -std=c++11 -I $PWD src/utilities.h src/models.h src/process_model.h src/measurement_model.h src/ukf.h test/main.cpp -o test/tests

cd ./test
./tests

