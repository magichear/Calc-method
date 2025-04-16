#ifndef GAUSS_SEIDEL_SOLVER_H
#define GAUSS_SEIDEL_SOLVER_H

#include "Solver.hh"
#include <vector>

class GaussSeidelSolver : public Solver {
public:
    GaussSeidelSolver(const std::vector<std::vector<Value>> A,const std::vector<Value> b, Value eps = 1e-7, int maxSteps = 10000);

    void solve() override;
};

#endif