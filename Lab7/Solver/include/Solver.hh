#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Config.hh"

using namespace std;

/**
 * @brief 从Lab5搓的框架搬过来的求解器接口结构
 */
class Solver {
protected:
    vector<vector<Value>> A;
    vector<Value> b;
    vector<Value> x;
    int max_steps;
    Value epsilon;
    int   steps;
    virtual void solve() = 0;    // 派生类中可以移到公有域以支持外部调用，也可以保持私有然后覆盖print方法。这里因为有SOR的比较策略，求解完直接输出不太合适，所以选择前者
    void updateInfNorm(Value & InfNorm, Value newValue);
public:
    Solver(const vector<vector<Value>> A,const vector<Value> b, Value eps = 1e-7, int maxSteps = 1000);

    void print() const;
    void check() const;

    int getMaxsteps() const { return max_steps; }
    int getsteps()    const { return steps;     }

};

#endif