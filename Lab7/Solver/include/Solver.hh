#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <iostream>
#include <iomanip>
#include "Config.hh"
using namespace std;

class Solver {
protected:
    vector<vector<Value>> A;
    vector<Value> b;
    vector<Value> x;
    int max_iterations;
    Value epsilon;

public:
    Solver(vector<vector<Value>> matrix, vector<Value> rhs, Value eps = 1e-7, int max_iter = 1000);

    virtual void solve() = 0;

    void printSolution() const;

    vector<Value> getSolution() const;
};

#endif