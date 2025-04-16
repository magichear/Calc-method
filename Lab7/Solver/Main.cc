#include "GaussSeidelSolver.hh"
#include "SORSolver.hh"
#include <memory>
#include <vector>

int main() {
    // 初始化矩阵和右端向量
    vector<vector<Value>> A = {
        { 31, -13,   0,   0,   0, -10,   0,  0,  0},
        {-13,  35,  -9,   0, -11,   0,   0,  0,  0},
        {  0,  -9,  31, -10,   0,   0,   0,  0,  0},
        {  0,   0, -10,  79, -30,   0,   0,  0, -9},
        {  0,   0,   0, -30,  57,  -7,   0, -5,  0},
        {  0,   0,   0,   0,  -7,  47, -30,  0,  0},
        {  0,   0,   0,   0,   0, -30,  41,  0,  0},
        {  0,   0,   0,   0,  -5,   0,   0, 27, -2},
        {  0,   0,   0,  -9,   0,   0,   0, -2, 29}
    };
    vector<Value> b = {-15, 27, -23, 0, -20, 12, -7, 7, 10};

    // (1) Gauss-Seidel迭代
    GaussSeidelSolver gs_solver(A, b);
    gs_solver.solve();
    cout << "Gauss-Seidel Results (" << gs_solver.getsteps() << " steps):" << endl;
    gs_solver.print();
    gs_solver.check();

    // (2) SOR迭代寻找最佳松弛因子
    Value best_omega = 1.0;
    int min_steps = INT_MAX;
    std::unique_ptr<SORSolver> best_solver;

    cout << "Testing different omega values:\n";

    for (int i = 1; i <= 100; ++i) { // 测试ω从0.02到2.0 <-- \omega 收敛的必要条件
        Value omega = i / 50.0;
        auto sor_solver = std::make_unique<SORSolver>(A, b, omega);
        sor_solver->solve();
        int steps = sor_solver->getsteps();

        if (steps >= sor_solver->getMaxsteps()) {
            cout << "Omega = " << omega << ": did not converge.\n";
        } else {
            cout << "Omega = " << omega << ": " << steps << " steps.\n";
        }

        if (steps < min_steps && steps > 0) {
            min_steps = steps;
            best_omega = omega;
            best_solver = std::move(sor_solver); // 保存当前最优解对应的求解器对象
        }
    }


    cout << "\nSOR Optimal Omega: " << best_omega << " (" << min_steps << " steps)" << endl;
    cout << "Solution with optimal omega:" << endl;
    best_solver->print();
    best_solver->check();

    return 0;
}