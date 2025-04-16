#include "GaussSeidelSolver.hh"
#include <cmath>
#include <algorithm>

GaussSeidelSolver::GaussSeidelSolver(std::vector<std::vector<Value>> A, std::vector<Value> b, Value eps, int maxSteps)
    : Solver(A, b, eps, maxSteps){}

void GaussSeidelSolver::solve() {
    Value InfNorm = 1.0;    // 无穷范数，在迭代过程中逐渐计算出来
    int n = A.size();
    steps = 0;    
    Value pre;

    while (InfNorm > epsilon && ++steps < max_steps) {
        InfNorm = 0.0;
        for (int i = 0; i < n; ++i) {
            Value s = 0.0;
            pre = x[i];
            for (int j = 0; j < n; ++j) { s += A[i][j] * x[j]; }// 这时候新值只更新完了第i-1个，后面都是旧的
            x[i] = (b[i] - s + A[i][i]*x[i]) / A[i][i]; // 需要去掉自己
            updateInfNorm(InfNorm, x[i] - pre);
        }
    }
}