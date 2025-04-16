#include "SORSolver.hh"
#include <cmath>
#include <algorithm>

SORSolver::SORSolver(std::vector<std::vector<Value>> matrix, std::vector<Value> rhs, Value w, Value eps, int max_iter)
    : Solver(matrix, rhs, eps, max_iter), omega(w), iterations(0) {}

void SORSolver::solve() {
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
            Value x_new = (1 - omega) * x[i] + omega * (b[i] - sigma) / A[i][i];
            max_diff = std::max(max_diff, std::abs(x_new - x[i]));
            x[i] = x_new;
        }

        iterations++;
        if (max_diff < epsilon) break;
    }
}

int SORSolver::getIterations() const { return iterations; }