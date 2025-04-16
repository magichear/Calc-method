#ifndef SOR_SOLVER_H
#define SOR_SOLVER_H

#include "Solver.hh"
#include <vector>

class SORSolver : public Solver {
private:
    Value omega;
    int iterations;

public:
    SORSolver(std::vector<std::vector<Value>> matrix, std::vector<Value> rhs, Value w, Value eps = 1e-7, int max_iter = 1000);

    void solve() override;

    int getIterations() const;
};

#endif