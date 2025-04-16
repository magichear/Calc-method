#include "Solver.hh"

Solver::Solver(vector<vector<Value>> A, vector<Value> b, Value eps, int maxSteps)
    : A(A), b(b), max_steps(maxSteps), epsilon(eps), steps(0) {
    x.resize(b.size(), 1.0);
}

void Solver::updateInfNorm(Value & InfNorm, Value newValue) {
    InfNorm = std::max(InfNorm, FABS(newValue));
}

void Solver::print() const {
    cout << fixed << setprecision(15);
    for (size_t i = 0; i < x.size(); ++i) {
        cout << left << "x" << setw(2) << (i + 1) << " = " << setw(20) << x[i];
        if ((i + 1) % 3 == 0) {
            cout << endl;
            continue;
        }
        if (i != x.size() - 1) cout << ", ";
    }
    cout << endl;
}

/**
 * 这两种方法不会修改A和b，所以不必担心内容不一致
 */
void Solver::check() const {
    cout << "\nCheck:" << endl;
    for (size_t i = 0; i < A.size(); ++i) {
        Value sum = 0.0;
        for (size_t j = 0; j < A[i].size(); ++j) {
            sum += A[i][j] * x[j];
        }
        cout << "Row " << setw(2) << left << (i + 1) << ": "
             << setw(20) << fixed << setprecision(12) << sum
             << "Real:" << setw(20) << b[i] << endl;
    }
    cout << "\n\n" << endl;
}