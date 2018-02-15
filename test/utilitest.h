#ifndef UTILITEST_H_
#define UTILITEST_H_
#include <iostream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "../src/state_vector.h"
#include "../src/matrix.h"
#include "../src/sigma_points.h"


namespace {
    const double EPSILON = 0.0001;
}


typedef std::pair<mtx::Matrix<double>*,mtx::Matrix<double>*> MatrixPair;
typedef std::vector<MatrixPair> MatrixPairs;


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
        static std::vector<std::string> readlines(std::string path) {
            std::ifstream file(path);
            std::string line;
            std::vector<std::string> lines;
            if (file.is_open()) {
                while (std::getline(file,line)) {
                    lines.push_back(line);
                }
                file.close();
            }
            return lines;
        }


        /*

        static std::vector<std::pair<mtx::Matrix<double>,mtx::Matrix<double> > >
            processMatrices(std::vector<std::string>& lines)
        {
            auto iter = lines.begin();
            std::vector<std::pair<mtx::Matrix<double>,mtx::Matrix<double> > > cmp;
            while (iter != lines.end()) {
                mtx::Matrix<double> q = processMatrix(*iter);
                ++iter;
                mtx::Matrix<double> a = processMatrix(*iter);
                ++iter;
                cmp.push_back(std::make_pair(q,a));

                std::pair<mtx::Matrix

            }
            return cmp;
        }

        */



        /*
        static std::vector<std::map<char,mtx::Matrix<double> > >
            processMatrices(std::vector<std::string>& lines)
        {
            auto iter = lines.begin();
            std::vector<std::map<char,mtx::Matrix<double> > > cmp;
            while (iter != lines.end()) {

                mtx::Matrix<double> q = processMatrix(*iter);
                ++iter;
                mtx::Matrix<double> a = processMatrix(*iter);
                ++iter;
                std::map<char,mtx::Matrix<double> > elem = {
                    {'q',q},
                    {'a',a}
                };
                cmp.push_back(elem);
            }

            return cmp;
        }
        */

        /*

        static MatrixPairs processMatrices(
            std::vector<std::string>& lines)
        {

            //std::vector<mtx::Matrix<double>* > matrices;


            MatrixPairs matrices;


            for (int i = 0; i < lines.size(); i += 2) {

                mtx::Matrix<double> Q(0,0);
                mtx::Matrix<double> A(0,0);

                processMatrix(lines[i],Q);
                processMatrix(lines[i+1],A);

                Q.print();

                matrices.push_back(
                    std::make_pair(&Q,&A)
                );

            }

            return matrices;
        }

        */


        static mtx::Matrix<double> processMatrix(std::string& str) {
            int len = str.size(),index = 6,
                r = 2,c = 4,m = 0;
            int rows = str[r]-'0',
                cols = str[c]-'0';
            mtx::Matrix<double> matrix(rows,cols);
            for (int i = index; i < len; ++i) {
                int sum = 0;
                char ch = str[i];
                while (ch != ',') {
                    sum = sum*10 + ch-'0';
                    ch = str[++i];
                }
                matrix[m++] = sum;
            }
            return matrix;
        }


        static std::vector<std::pair<mtx::Matrix<double>,mtx::Matrix<double> > >
            procesMatrices(std::string filepath)
        {

            std::vector<std::pair<mtx::Matrix<double>,mtx::Matrix<double> > > res;
            std::vector<std::string> lines = readlines(filepath);
            for (int i = 0; i < lines.size(); i += 2) {

                mtx::Matrix<double> q = processMatrix(lines[i]);
                mtx::Matrix<double> a = processMatrix(lines[i+1]);
                res.push_back(std::make_pair(q,a));


            }

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

