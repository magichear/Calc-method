#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "Config.hh"

class Solver {
protected:
    struct Result {
        bool  success    = false;
        Value root       = 0.0;
        int   steps      = 0;
        std::vector<Value> iterates;
    };
    // 迭代过程处理（非必要，这里仅用于展示迭代过程，便于后续分析计算收敛阶）
    static void dealWithIterate(const std::vector<Value>& iterates, Value root);
    std::vector<std::vector<Value>> initials;

private:
    // 求解器（必选实现）
    virtual Result solve(const std::vector<Value>& initial, Value epsilon) = 0;

    // 模块标识（必选实现）
    virtual std::string getMethodName() = 0;

public:
    virtual ~Solver() = default;

    // 设置初值（无需实现）
    void setInitials(std::vector<std::vector<Value>>&& initials);
    // 计算并打印结果（可选实现）
    void print(Value epsilon);
};

#endif