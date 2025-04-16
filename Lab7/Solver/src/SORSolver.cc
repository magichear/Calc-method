#include "SORSolver.hh"
#include <cmath>
#include <algorithm>

SORSolver::SORSolver(std::vector<std::vector<Value>> A, std::vector<Value> b, Value omega, Value eps, int maxSteps)
    : Solver(A, b, eps, maxSteps), omega(omega) {}

void SORSolver::solve() {
    Value InfNorm = 1.0;    // 无穷范数，在迭代过程中逐渐计算出来
    int n = A.size();
    steps = 0;    
    Value pre;

    while (InfNorm > epsilon && ++steps < max_steps) {    // 这里将迭代步数自增逻辑后置，这样如果中间满足精度要求退出之后步数不会多记一次（短路）
        InfNorm = 0.0;
        for (int i = 0; i < n; ++i) {
            Value sum = 0.0;
            pre = x[i];
            for (int j = 0; j < n; ++j) { sum += A[i][j] * x[j]; }
            x[i] = (1 - omega) * x[i] + (omega * (b[i] - sum + A[i][i]*x[i])) / A[i][i];    // 先把分子乘大一点再除
            updateInfNorm(InfNorm, x[i] - pre);
        }
    }
}