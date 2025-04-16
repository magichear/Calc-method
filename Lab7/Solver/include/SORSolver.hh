#ifndef SOR_SOLVER_H
#define SOR_SOLVER_H

#include "Solver.hh"
#include <vector>

/**
 * @brief 逐次超松弛迭代
 */
class SORSolver : public Solver {
private:
    Value omega;

public:
    SORSolver(const std::vector<std::vector<Value>> A,const std::vector<Value> b, Value omega, Value eps = 1e-7, int maxSteps = 100000);

    void solve() override;
};

#endif