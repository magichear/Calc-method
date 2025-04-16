#include "Solver.hh"

Solver::Solver(vector<vector<Value>> matrix, vector<Value> rhs, Value eps, int max_iter)
    : A(matrix), b(rhs), epsilon(eps), max_iterations(max_iter) {
    x.resize(b.size(), 1.0);
}

void Solver::printSolution() const {
    cout << fixed << setprecision(9);
    for (size_t i = 0; i < x.size(); ++i) {
        cout << "x" << (i + 1) << " = " << x[i];
        if (i != x.size() - 1) cout << ", ";
        if ((i + 1) % 3 == 0) cout << endl << " ";
    }
    cout << endl;
}

vector<Value> Solver::getSolution() const {
    return x;
}