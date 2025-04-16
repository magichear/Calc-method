#include "GaussSeidelSolver.hh"
#include <cmath>
#include <algorithm>

GaussSeidelSolver::GaussSeidelSolver(std::vector<std::vector<Value>> matrix, std::vector<Value> rhs, Value eps, int max_iter)
    : Solver(matrix, rhs, eps, max_iter), iterations(0) {}

void GaussSeidelSolver::solve() {
    int n = A.size();
    std::vector<Value> x_old = x;
    iterations = 0;

    while (iterations < max_iterations) {
        x_old = x;
        Value max_diff = 0.0;

        for (int i = 0; i < n; ++i) {
            Value sigma = 0.0;
            for (int j = 0; j < n; ++j) {
                if (j != i) sigma += A[i][j] * x[j];
            }
            x[i] = (b[i] - sigma) / A[i][i];
            max_diff = std::max(max_diff, std::abs(x[i] - x_old[i]));
        }

        iterations++;
        if (max_diff < epsilon) break;
    }
}

int GaussSeidelSolver::getIterations() const { return iterations; }