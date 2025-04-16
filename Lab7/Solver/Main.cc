#include "NewtonSolver.hh"
#include "SecantSolver.hh"
#include <memory>
#include <vector>

int main() {
    // 初始化矩阵和右端向量
    vector<vector<Value>> A = {
        {31, -13, 0, 0, 0, -10, 0, 0, 0},
        {-13, 35, -9, 0, -11, 0, 0, 0, 0},
        {0, -9, 31, -10, 0, 0, 0, 0, 0},
        {0, 0, -10, 79, -30, 0, 0, 0, -9},
        {0, 0, 0, -30, 57, -7, 0, -5, 0},
        {0, 0, 0, 0, -7, 47, -30, 0, 0},
        {0, 0, 0, 0, 0, -30, 41, 0, 0},
        {0, 0, 0, 0, -5, 0, 0, 27, -2},
        {0, 0, 0, -9, 0, 0, 0, -2, 29}
    };
    vector<Value> b = {-15, 27, -23, 0, -20, 12, -7, 7, 10};

    // (1) Gauss-Seidel迭代
    GaussSeidelSolver gs_solver(A, b);
    gs_solver.solve();
    cout << "Gauss-Seidel Results (" << gs_solver.getIterations() << " iterations):" << endl;
    gs_solver.printSolution();

    // (2) SOR迭代寻找最佳松弛因子
    Value best_omega = 1.0;
    int min_steps = numeric_limits<int>::max();
    vector<Value> best_solution;

    for (int i = 1; i <= 100; ++i) { // 测试ω从0.02到2.0
        Value omega = i / 50.0;
        SORSolver sor_solver(A, b, omega);
        sor_solver.solve();
        int steps = sor_solver.getIterations();

        if (steps < min_steps && steps > 0) {
            min_steps = steps;
            best_omega = omega;
            best_solution = sor_solver.getSolution();
        }
    }

    cout << "\nSOR Optimal Omega: " << best_omega << " (" << min_steps << " iterations)" << endl;
    cout << "Solution with optimal omega:" << endl;
    cout << fixed << setprecision(9);
    for (size_t i = 0; i < best_solution.size(); ++i) {
        cout << "x" << (i+1) << " = " << best_solution[i];
        if (i != best_solution.size()-1) cout << ", ";
        if ((i+1) % 3 == 0) cout << endl << " ";
    }
    cout << endl;

    return 0;
}