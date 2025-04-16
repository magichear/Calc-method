#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>

using namespace std;

class Solver {
protected:
    vector<vector<double>> A;
    vector<double> b;
    vector<double> x;
    int max_iterations;
    double epsilon;

public:
    Solver(vector<vector<double>> matrix, vector<double> rhs, double eps = 1e-7, int max_iter = 1000)
        : A(matrix), b(rhs), epsilon(eps), max_iterations(max_iter) {
        x.resize(b.size(), 0.0);
    }

    virtual void solve() = 0;

    void printSolution() const {
        cout << fixed << setprecision(9);
        for (size_t i = 0; i < x.size(); ++i) {
            cout << "x" << (i+1) << " = " << x[i];
            if (i != x.size()-1) cout << ", ";
            if ((i+1) % 3 == 0) cout << endl << " ";
        }
        cout << endl;
    }

    vector<double> getSolution() const { return x; }
};

class GaussSeidelSolver : public Solver {
private:
    int iterations;

public:
    GaussSeidelSolver(vector<vector<double>> matrix, vector<double> rhs, double eps = 1e-7, int max_iter = 1000)
        : Solver(matrix, rhs, eps, max_iter), iterations(0) {}

    void solve() override {
        int n = A.size();
        vector<double> x_old = x;
        iterations = 0;

        while (iterations < max_iterations) {
            x_old = x;
            double max_diff = 0.0;

            for (int i = 0; i < n; ++i) {
                double sigma = 0.0;
                for (int j = 0; j < n; ++j) {
                    if (j != i) sigma += A[i][j] * x[j];
                }
                x[i] = (b[i] - sigma) / A[i][i];
                max_diff = max(max_diff, abs(x[i] - x_old[i]));
            }

            iterations++;
            if (max_diff < epsilon) break;
        }
    }

    int getIterations() const { return iterations; }
};

class SORSolver : public Solver {
private:
    double omega;
    int iterations;

public:
    SORSolver(vector<vector<double>> matrix, vector<double> rhs, double w, double eps = 1e-7, int max_iter = 1000)
        : Solver(matrix, rhs, eps, max_iter), omega(w), iterations(0) {}

    void solve() override {
        int n = A.size();
        vector<double> x_old = x;
        iterations = 0;

        while (iterations < max_iterations) {
            x_old = x;
            double max_diff = 0.0;

            for (int i = 0; i < n; ++i) {
                double sigma = 0.0;
                for (int j = 0; j < n; ++j) {
                    if (j != i) sigma += A[i][j] * x[j];
                }
                double x_new = (1 - omega) * x[i] + omega * (b[i] - sigma) / A[i][i];
                max_diff = max(max_diff, abs(x_new - x[i]));
                x[i] = x_new;
            }

            iterations++;
            if (max_diff < epsilon) break;
        }
    }

    int getIterations() const { return iterations; }
};

int main() {
    // 初始化矩阵和右端向量
    vector<vector<double>> A = {
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
    vector<double> b = {-15, 27, -23, 0, -20, 12, -7, 7, 10};

    // (1) Gauss-Seidel迭代
    GaussSeidelSolver gs_solver(A, b);
    gs_solver.solve();
    cout << "Gauss-Seidel Results (" << gs_solver.getIterations() << " iterations):" << endl;
    gs_solver.printSolution();

    // (2) SOR迭代寻找最佳松弛因子
    double best_omega = 1.0;
    int min_steps = numeric_limits<int>::max();
    vector<double> best_solution;

    for (int i = 1; i <= 100; ++i) { // 测试ω从0.02到2.0
        double omega = i / 50.0;
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