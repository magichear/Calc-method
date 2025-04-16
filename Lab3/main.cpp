#include <iostream>
#include <vector>  
#include <iomanip> 

using namespace std;

/**
 * 这里x比较小，泰勒收敛快，而且这次数据long double的结果跟double的差不多
 */
class Node {
private:
    long double x;
    long double y;
    long double sinX;
    long double cosX;

    long double sin(long double x) {
        int         n    = 1;    
        long double res  = x;    
        long double term = x;
        long double xx   = x * x;

        while (true) {
            term *= -xx / ((2 * n) * (2 * n + 1));
            if (abs(term) < 1e-32) break;
            res += term;
            n++;
        }
        return res;
    }

    // 一开始这里直接复制了sin的参数初值，然后算出均方误差比实际小了一个数量级LOL
    long double cos(long double x) {
        int         n    = 1  ;    
        long double res  = 1.0;    
        long double term = 1.0;
        long double xx   = x * x;

        while (true) {
            term *= -xx / ((2 * n - 1) * (2 * n));
            if (abs(term) < 1e-32) break;
            res += term;
            n++;
        }
        return res;
    }

public:
    long double get_x()    const { return x;    }
    long double get_y()    const { return y;    }
    long double get_sinX() const { return sinX; }
    long double get_cosX() const { return cosX; }
    Node(long double x_val, long double y_val) : x(x_val), y(y_val), sinX(sin(x_val)), cosX(cos(x_val)){}
};

/**
 * 将均方误差分别对a, b求偏导后得到下面的法方程
 * \sum y_i cos(x_i) = a \sum sin(x_i)cos(x_i) + b \sum cos^2(x_i)
 * \sum y_i sin(x_i) = a \sum sin^2(x_i)       + b \sum sin(x_i)cos(x_i)
 */
class Ass03 {
private:
    long double  a;
    long double  b;
    long double  mse;
    vector<Node> points;    

    void calc() {
        long double yCos   = 0.0, ySin   = 0.0;    
        long double sinSin = 0.0, cosCos = 0.0, sinCos = 0.0;

        for (const auto& point : points) {
            long double sine   = point.get_sinX();
            long double cosine = point.get_cosX(); 

            sinSin += sine * sine;
            sinCos += sine * cosine;
            cosCos += cosine * cosine;                        
            ySin   += point.get_y() * sine;
            yCos   += point.get_y() * cosine;
        }

        // 克拉默法则
        long double D = sinSin * cosCos - sinCos * sinCos;   // 行列式
        a = (ySin * cosCos - yCos * sinCos) / D;             // 替换第一列为常数项后的行列式 <-------常数项就是y那边的系数
        b = (yCos * sinSin - ySin * sinCos) / D;             // 替换第二列为常数项后的行列式

        mse = 0.0;
        long double diff = 0.0;
        for (const auto& point : points) {
            diff = point.get_y() - calcY(point);
            mse += diff * diff;
        }
        mse /= points.size(); 
    }

    long double calcY(Node point) {
        return a * point.get_sinX() + b * point.get_cosX();
    }

public:
    Ass03(const vector<Node>& points) : points(points), a(0.0), b(0.0), mse(0.0) {
        calc();
    }

    void print() const {
        cout << endl;
        cout << scientific << setprecision(15);
        cout << "a = " << a << ", b = " << b << ", MSE=" << mse << endl;
        cout << endl;
    }
};

int main() {
    vector<Node> points = {
        Node(0.25, 1.284), Node(0.50, 1.648),
        Node(0.75, 2.117), Node(1.00, 2.718),
        Node(1.25, 3.427), Node(1.50, 2.798),
        Node(1.75, 3.534), Node(2.00, 4.456),
        Node(2.25, 5.465), Node(2.50, 5.894)
    };

    Ass03(points).print();

    return 0;
}