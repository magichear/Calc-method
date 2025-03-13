#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>

#define PI 3.141592653589793238462643383279502884L

/**
 * Lagrange 插值误差比较器，允许输入插值函数阶数、原始函数（必选）、插值节点生成器（可选）
 * 
 * 对于n阶插值 L_n (x)，需要使用n+1个节点
 */
class LagrangeErrComparator {
private:
    int n;                                              // 节点数量
    std::vector<long double> nodes_x;                   // 插值节点的 x 坐标
    std::vector<long double> nodes_fx;                  // 插值节点的 f(x) 值
    std::vector<long double> denominators;              // 拉格朗日基函数各项的分母
    std::function<long double(int)> nodeFunc;           // 插值节点生成方法的指针    
    std::function<long double(long double)> originFunc; // 原始函数的指针

    /**
     * 计算拉格朗日插值多项式在 y 处的值
     * O(n)
     */
    long double lagrange(long double y) const {
        long double res = 0.0L;
        size_t n = nodes_x.size();
        std::vector<long double> prefix, suffix;
        __fixGen(y, prefix, suffix);
    
        // 计算插值多项式
        for (size_t i = 0; i < n; ++i) {
            long double numerator = prefix[i] * suffix[i];
            res += nodes_fx[i] * (numerator / denominators[i]);
        }
    
        return res;
    }

    // 初始化所有确定值
    void __init(int nodeTy) {
        if (!nodeFunc) {
            switch (nodeTy) {
                case 0: nodeFunc = std::bind(&LagrangeErrComparator::__eqGen, this, std::placeholders::_1); break;
                case 1: nodeFunc = std::bind(&LagrangeErrComparator::__chGen, this, std::placeholders::_1); break;
                default: throw std::runtime_error("Node type(0/1) or nodeGen function is required.");
            }        
        }
        __nodeGen();
        __denomGen();
    }

    /**
     * 用于为lagrange生成前缀积与后缀积，改善计算效率
     */
    void __fixGen(long double y, std::vector<long double>& prefix, std::vector<long double>& suffix) const {
        size_t n = nodes_x.size();
        prefix.resize(n, 1.0L);
        suffix.resize(n, 1.0L);
        std::vector<long double> diffs_(n, 1.0L);

        for (size_t i = 0; i < n; ++i) {
            diffs_[i] = y - nodes_x[i];
        }
        prefix[0] = 1.0L;
        suffix[n - 1] = 1.0L;
        for (size_t i = 1; i < n; ++i) {
            prefix[i] = prefix[i - 1] * diffs_[i-1];
        }
        for (size_t i = n - 2; i < n; --i) {
            suffix[i] = suffix[i + 1] * diffs_[i+1];
        }
    }

    /**
     * 生成拉格朗日插值基函数的分母
     * 
     * 分子需要结合待计算的x值采才能算
     */
    void __denomGen() {
        denominators.clear();
        denominators.resize(nodes_x.size());
        for (size_t i = 0; i < nodes_x.size(); ++i) {
            long double denom = 1.0L;
            for (size_t k = 0; k < nodes_x.size(); ++k) {
                if (k != i) denom *= (nodes_x[i] - nodes_x[k]);
            }
            denominators[i] = denom;
        }
    }

    /**
     * 生成插值节点及其对应位置的原始函数值
     */
    void __nodeGen() {
        nodes_x.clear();
        nodes_fx.clear();
        for (int i = 0; i <= n; ++i) {
            long double x;
            x = nodeFunc(i);
            nodes_x.push_back(x);
            nodes_fx.push_back(originFunc(x)); 
        }
    }

    long double __eqGen(int i) {
        return -5.0L + 10.0L * i / n;
    }

    long double __chGen(int i) {
        return -5.0L * std::cos((2 * i + 1) * PI / (2 * (n + 1)));
    }

/*
这里均匀取结点发现Equidist基本没区别，切比雪夫的计算太慢了
void __nodeGen() {
    nodes_x.clear();
    nodes_fx.clear();
    int num = __nodeCnt(); // 获取节点总数
    int step_size = num / n; // 计算步长

    for (int i = 0; i <= n; ++i) {
        int index = i * step_size;
        long double x = nodeFunc(index);
        nodes_x.push_back(x);
        nodes_fx.push_back(originFunc(x));
    }
}

int __nodeCnt() {
    for (int i = n; ; i++) {
        if (nodeFunc(i) >= 5.0L) return i+1;
    }
}
*/

public:
    /**
     * @param order 插值函数的阶数
     * @param nodeTy 插值节点生成方法，0: 均匀取点，1: 切比雪夫节点
     * @param func 原始函数
     * @param nodeFunc 插值节点生成方法
     */
    LagrangeErrComparator(int order, int nodeTy, std::function<long double(long double)> func, std::function<long double(int)> nodeFunc = nullptr) 
        : n(order), originFunc(func), nodeFunc(nodeFunc) {
        __init(nodeTy);
    }

    /**
     * @param tests 全部测试点
     * @return 最大模误差 max{||f - L||} \approx max{|f - L|}
     */
    long double findMME(const std::vector<long double>& tests) const {
        long double max = 0.0L;
        for (const auto& y : tests) {
            long double err = Compute(y);
            if (err > max) max = err;
        }
        return max;
    }    

    // 计算单点误差
    long double Compute(long double y) const {
        return std::abs(originFunc(y) - lagrange(y));
    }    

};

int main() {
    std::vector<int> n_values = {5, 10, 20, 40};  // 插值函数的阶数
    std::vector<long double> tests;
    // 原始函数
    std::function<long double(long double)> func = [](long double x) { return 1.0L / (1.0L + x * x); };

    // 生成测试点
    for (int j = 0; j <= 500; ++j) {
        tests.push_back(-5.0L + 10.0L * j / 500.0L);
    }

    auto compute_and_print = [&](int n, int nodeTy) {
        LagrangeErrComparator interpolator(n, nodeTy, func, nullptr);
        long double max_err = interpolator.findMME(tests);

        std::cout << "Node count = " << n+1
                  << ", Max mod Error = " << std::scientific << std::setprecision(15)
                  << max_err << std::endl;
    };

    // 计算并输出结果
    std::cout << "Equidist" << std::endl;
    for (int n : n_values) {
        compute_and_print(n, 0);
    }
    std::cout << std::endl;

    // 计算并输出结果
    std::cout << "Chebyshev" << std::endl;
    for (int n : n_values) {
        compute_and_print(n, 1);
    }
    std::cout << std::endl;

    system("pause");
    return 0;
}