#ifndef SECANTSOLVER_H
#define SECANTSOLVER_H

#include "Solver.hh"
#include <functional>

class SecantSolver : public Solver {
private:
    std::function<Value(Value)> f; // 非线性方程 f(x)
    
    Result solve(const std::vector<Value>& initial, Value epsilon) override;
    std::string getMethodName() override;

public:
    SecantSolver(std::function<Value(Value)> func);
};

#endif