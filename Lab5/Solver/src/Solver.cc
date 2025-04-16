#include "Solver.hh"
#include "Config.hh"

void Solver::setInitials(std::vector<std::vector<Value>>&& initials) {
    this->initials = std::move(initials);
}

void Solver::dealWithIterate(const std::vector<Value>& iterates, Value root) {
    for (auto iterate : iterates) {
        std::cout << std::setprecision(16)  << std::left << FABS(root - iterate) << std::endl;
    }
}

void Solver::print(Value epsilon) {
    std::cout << getMethodName() << " Results:\n";
    for (const auto& initial : initials) {
        auto res = solve(initial, epsilon);
        //dealWithIterate(res.iterates, res.root);
        std::cout << "Initial Values = ";
        if (initial.size() == 1) {
            std::cout << std::setprecision(1) << std::setw(14) << std::left << initial[0];
        } else {
            std::cout << "(" << std::setprecision(1) << std::setw(5) << std::left << initial[0] << ", "
                      << std::setw(5) << std::left << initial[1] << ")";
        }
        std::cout << ", Root = " << std::fixed << std::setprecision(15) << std::setw(20) << std::left << res.root
                  << ", steps = " << std::setw(3) << std::left << res.steps;
        std::cout << std::endl;
    }
}