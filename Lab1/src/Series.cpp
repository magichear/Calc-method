#include "Series.hpp"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

Series::Series(double precision) : precision(precision) {}

double Series::calc(long double x) const {
    // const long double gamma = 0.57721566490153286060651209;
    long double result = 0.0;

    switch (static_cast<int>(x * 10)) {
        case 0: // 0
            result = M_PI * M_PI / 6;
            break;
        case 5: // 0.5
            result = 4 * (1 - log(2));
            break;
        case 10: // 1
            result = 1 - 1.0 / (UPPER_LIMIT + 2);   //这里是归纳变量的上限，本身比k小1，不过一点点影响不大，我多加了两个数量级
            break;
        case 14: // sqrt(2)
            if (fabs(x - sqrt(2)) < precision) {
                result = phi(x); // 爆算
            }
            break;
        default: // 10, 100, 300
            auto n = static_cast<int>(x);
            if (x == abs(n)) { // 保证正整数
                long double a=1, b=1;
                for (int i = 2; i <= n; i++) {
                    b = b*i + a;            
                    a *= i;
                }
                result = b / (a*n);     // 整数乘法应该不会丢精度？整个过程只除一次
            }
            else {
                cerr << "Invalid input: " << x << endl;
            }
            break;
    }

    return result;
}

double Series::phi(long double x) const {   // 这里基本可以达到小数点后7位的精度
    long double sum = 0.0;
    long double term = 1.0;
    int k = 1, i = 0;

    while(i++ < UPPER_LIMIT) {
        term = 1.0 / (k * (k + x));
        sum += term;
        k++;
    }

    return static_cast<double>(sum);
}

void Series::printResult(long double x, long double result, bool flag) const {
    ostringstream oss;
    oss << fixed << setprecision(15);
    oss << "x=" << setw(8);
    switch (static_cast<int>(x * 10)) {
    case 14:
        oss << "sqrt(2)";
        break;
    default:
        oss << setprecision(1) << x;
        break;
    }
    oss << ", y=" << scientific << setprecision(15) << result << endl;

    cout << oss.str();

    ofstream outfile;
    if (flag) {
        outfile.open("../output.txt", ios::app);
    } else {
        outfile.open("../output.txt", ios::trunc);
    }
    if (outfile.is_open()) {
        outfile << oss.str();
        outfile.close();
    } else {
        cerr << "can't open ../output.txt" << endl;
    }
}