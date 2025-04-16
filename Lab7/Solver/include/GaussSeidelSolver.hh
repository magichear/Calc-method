#ifndef GAUSS_SEIDEL_SOLVER_H
#define GAUSS_SEIDEL_SOLVER_H

#include "Solver.hh"
#include <vector>

class GaussSeidelSolver : public Solver {
private:
    int iterations;

public:
    GaussSeidelSolver(std::vector<std::vector<Value>> matrix, std::vector<Value> rhs, Value eps = 1e-7, int max_iter = 1000);

    void solve() override;

    int getIterations() const;
};

#endif