#include "SecantSolver.hh"

SecantSolver::SecantSolver(std::function<Value(Value)> func) : f(func) {}

// 严格按照书上伪代码实现，另外补充了除零错误处理
Solver::Result SecantSolver::solve(const std::vector<Value>& initial, Value epsilon) {
    Result result;
    if (initial.size() < 2) return result;

    Value x1 = initial[0], x2 = initial[1];
    int step = 2;
    const int MAXREPT = 1000;
    Value fx1 = f(x1), fx2;

    result.iterates.push_back(x1);
    result.iterates.push_back(x2);

    while (step++ < MAXREPT) {
        fx2 = f(x2);

        if (fx2 == fx1) break;  // 除0

        Value x = x2 - fx2 * (x2 - x1) / (fx2 - fx1);
        result.iterates.push_back(x);
        Value error = FABS(x - x2);

        if (error < epsilon || FABS(f(x)) < epsilon) {
            result.root = x;
            result.steps = step-2;
            result.success = true;
            break;
        }
        fx1 = fx2;
        x1 = x2;
        x2 = x;
    }
    return result;
}

std::string SecantSolver::getMethodName() {
    return "Secant";
}