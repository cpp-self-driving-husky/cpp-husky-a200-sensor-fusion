#ifndef UTILITEST_H_
#define UTILITEST_H_
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <iomanip>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "../src/state_vector.h"
#include "../src/matrix.h"
#include "../src/sigma_points.h"


namespace {
    const double EPSILON = 0.0001;
}


typedef std::vector<std::pair<mtx::Matrix<double>,mtx::Matrix<double> > > MatrixPairs;


struct test {


    private:
        template<typename T>
        static bool similar(T& a, T& b, double eps = EPSILON) {
            if (a >= b-eps && a <= b+eps)
                return true;
            return false;
        }


        template<typename T>
        static bool comparable(T& res, T& ans) {
            return equivalent(res,ans);
        }


        template<typename T>
        static bool comparable(mtx::Matrix<T>& res, mtx::Matrix<T>& ans) {
            int len = res.getSize();
            if (res.getRows() != ans.getRows() ||
                res.getCols() != ans.getCols())
                return false;
            for (int i = 0; i < len; ++i)
                if (!similar(res[i],ans[i]))
                    return false;
            return true;
        }


        template<typename T>
        static void console(T& a, T& b) {
            std::cout << "\n" << a << " " << b << "\n" << std::endl;
        }


        template<typename T>
        static void outline(T& a) {
            int w = 10;
            int p = 5;
            std::cout <<
                std::setprecision(p) <<
                std::setw(w) <<
                std::fixed <<
                std::left <<
                a << " ";
        }


        template<typename T>
        static void console(mtx::Matrix<T>& res, mtx::Matrix<T>& ans) {
            int lr = res.getRows(), lc = res.getCols(),
                rr = ans.getRows(), rc = ans.getCols();
            for (int i = 0; i < lr; ++i) {
                for (int j = 0; j < lc; ++j)
                    outline(res[i*lc+j]);
                std::cout << "   ";
                for (int j = 0; j < rc; ++j)
                    outline(ans[i*rc+j]);
                std::cout << "\n";
            }
            std::cout << std::endl;
        }


        static void successful() {
            std::cout << "\n" <<
                "Inputs are similar" << "\n\n";
        }


        static void unsuccessful() {
            std::cout << "\n"
                "Inputs are not similar" << "\n\n";
        }



    public:
        static std::vector<std::string> split(const std::string& s, char delimiter) {
           std::vector<std::string> tokens;
           std::string token;
           std::istringstream tokenStream(s);
           while (std::getline(tokenStream, token, delimiter))
              tokens.push_back(token);
           return tokens;
        }


    public:
        static std::vector<std::string> readlines(std::string path) {
            std::ifstream file(path);
            std::string line;
            std::vector<std::string> lines;
            if (file.is_open()) {
                while (std::getline(file,line))
                    lines.push_back(line);
                file.close();
            }
            return lines;
        }


        static int getDimension(std::string& data, int index) {
            int len = data.size(),
                sum = 0;
            for (int i = index; i < len && data[i] != '/'; ++i)
                sum = sum*10 + data[i]-'0';
            return sum;
        }


        static mtx::Matrix<double> processMatrix(std::string& str) {
            int r = 2,c = 4;
            int rows = getDimension(str,r),
                cols = getDimension(str,c);
            mtx::Matrix<double> matrix(rows,cols);
            std::vector<std::string> elements = split(str,',');
            elements.erase(elements.begin());
            for (int i = 0; i < elements.size(); ++i)
                matrix[i] = atof(elements[i].c_str());
            return matrix;
        }


        static MatrixPairs procesMatrices(std::string filepath) {
            std::vector<std::string> lines = readlines(filepath);
            MatrixPairs res;
            for (int i = 0; i < lines.size(); i += 2)
                res.push_back(std::make_pair(
                    processMatrix(lines[i]),
                    processMatrix(lines[i+1])));
            return res;
        }


    public:
        template<typename T>
        static void run(T& a, T& b) {
            if(comparable(a,b))
                successful();
            else
                unsuccessful();
            console(a,b);
        }

        template<typename T>
        static void run(mtx::Matrix<T>& res, mtx::Matrix<T>& ans) {
            if (comparable(res,ans))
                successful();
            else
                unsuccessful();
            console(res,ans);
        }


};


#endif // UTILITEST_H_

