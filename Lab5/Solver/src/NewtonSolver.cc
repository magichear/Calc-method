#include "NewtonSolver.hh"

NewtonSolver::NewtonSolver(std::function<Value(Value)> func, std::function<Value(Value)> deriv)
    : f(func), df(deriv) {}

// 严格按照书上伪代码实现，另外补充了除零错误处理
Solver::Result NewtonSolver::solve(const std::vector<Value>& initial, Value epsilon) {
    Result result;
    if (initial.empty()) return result;

    Value x_0 = initial[0];
    int step = 1;
    const int MAXREPT = 1000;

    auto g = [this](Value x, Value df) { return x - f(x) / df; };

    while (step++ < MAXREPT) {
        result.iterates.push_back(x_0);
        Value dfx = df(x_0);
        if (dfx == 0) break;

        Value x_1 = g(x_0, dfx);
        Value error = FABS(x_1 - x_0);

        if (error < epsilon) {
            result.root = x_1;
            result.steps = step-1;
            result.success = true;
            break;
        }
        x_0 = x_1;
    }
    return result;
}

std::string NewtonSolver::getMethodName() {
    return "Newton";
}