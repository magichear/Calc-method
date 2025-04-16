#ifndef NEWTONSOLVER_H
#define NEWTONSOLVER_H

#include "Solver.hh"
#include <functional>

class NewtonSolver : public Solver {
private:
    std::function<Value(Value)> f;  // 非线性方程 f(x)
    std::function<Value(Value)> df; // 非线性方程的导数 f'(x)

    Result solve(const std::vector<Value>& initial, Value epsilon) override;
    std::string getMethodName() override;
public:
    NewtonSolver(std::function<Value(Value)> func, std::function<Value(Value)> dfunc);
};

#endif