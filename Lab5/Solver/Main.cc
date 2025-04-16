#include "NewtonSolver.hh"
#include "SecantSolver.hh"
#include <memory>
#include <vector>

int main() {
    auto f = [](Value x) { return (POW(x, 3) / 3) - x; };
    auto df = [](Value x) { return x*x - 1; };

    // 这一部分其实还可以改成工厂，但是这次实验感觉还是没必要了
    auto newton = std::make_unique<NewtonSolver>(f, df);
    auto secant = std::make_unique<SecantSolver>(f);

    newton->setInitials({{0.1}, {0.2}, {0.9}, {9.0}});
    secant->setInitials({{-0.1, 0.1}, {-0.2, 0.2}, {-2.0, 0.9}, {0.9, 9.0}});

    const Value epsilon = 1e-8;

    std::vector<std::unique_ptr<Solver>> solvers;
    solvers.push_back(std::move(newton));
    solvers.push_back(std::move(secant));

    for (const auto& solver : solvers) {
        solver->print(epsilon);
        std::cout << std::endl;
    }

    return 0;
}