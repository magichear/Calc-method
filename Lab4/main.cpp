#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <memory>

using namespace std;

#define Value long double
class Integrator {
public:
    virtual Value calc(Value a, Value b, int N) = 0;
    virtual ~Integrator() = default;
};

/**
 * 复化梯形积分器
 * \dfrac{h}{2} [ f(a) + 2 \sum ^{n-1} _{i=1} f(x_i) + f(b) ]
 */
class TrapezoidalIntegrator : public Integrator {
public:
    Value calc(Value a, Value b, int N) override {
        if (N < 2) return 0.0;
        
        int n = N-1;
        Value h = (b - a) / n;
        Value sum = 0;
        Value x = a;
        for (int i = 1; i < n; ++i) {
            x += h;
            sum += sin(x);
        }
        return (sin(a) + 2*sum + sin(b)) * h / 2.0;
    }
};

/**
 * 复化Simpson积分器
 * \dfrac{h}{3} [ f(a) + 4 \sum ^{n-1} _{i=1,3,5...} f(x_{i}) + 2 \sum ^{n-2} _{i=2,4,6...} f(x_{i}) + f(b) ]
 */
class SimpsonIntegrator : public Integrator {
public:
    Value calc(Value a, Value b, int N) override {
        if ((N&1) != 1 || N < 2) return 0.0; // N-1必须为偶数才能计算

        int n = N-1;
        Value h = (b - a) / n;
        Value oddSum = 0.0;
        Value evenSum = 0.0;
        Value x = a;
        for (int i = 1; i < n; ++i) {
            x += h;
            if ((i&1) == 1) oddSum += sin(x);
            else evenSum += sin(x);
        }
        return (sin(a) + 4.0 * oddSum + 2.0 * evenSum + sin(b)) * h / 3.0;
    }
};

/**
 * 本次只需要计算k=2时的十一个误差阶 
 * 误差阶越大说明误差减少、精度提高得越快
 *
 * double的精度不够，会导致Simpson最后几位的误差阶波动
 * double:
 * 0.256640121764800       0.000000001359887       4.000126622110034
 * 0.256640120489899       0.000000000084985       3.999986807224332
 * 0.256640120410225       0.000000000005312       3.997198327796937
 * 0.256640120405246       0.000000000000333       4.025259952401738
 * 0.256640120404934       0.000000000000020       2.569365645670138
 * 0.256640120404917       0.000000000000003
 * long double:
 * 0.256640121764800       0.000000001359886       4.000126843042480
 * 0.256640120489899       0.000000000084985       4.000048347110856
 * 0.256640120410225       0.000000000005311       4.000276930941804
 * 0.256640120405245       0.000000000000332       4.004338633367542
 * 0.256640120404934       0.000000000000021       4.070588253176797
 * 0.256640120404915       0.000000000000001
 */
int main() {
    const Value exact = cos(1.0) - cos(5.0); // 原式不定积分为 -cos
    const Value a     = 1.0, b = 5.0;        // 积分区间 [a, b]
    const int groups  = 12;
    vector<int> N_cases;
    for (int l = 1; l <= groups; ++l)
        N_cases.push_back((1 << l) + 1);      // 生成点数 N = 2^l + 1

    vector<unique_ptr<Integrator>> integrators;
    integrators.push_back(make_unique<TrapezoidalIntegrator>());
    integrators.push_back(make_unique<SimpsonIntegrator>());
    bool flag = true;

    cout << fixed << setprecision(15);
    cout << "\n\t\tExact Value: " << exact << endl;
    for (const auto& integrator : integrators) {
        vector<Value> values, errors, orders;
        for (int N : N_cases) {               // 节点数接近倍增，步长相应的近似看作减半
            Value val = integrator->calc(a, b, N);
            values.push_back(val);
            errors.push_back(fabs(exact - val));
        }

        // 误差阶
        for (int i = 0; i < groups-1; ++i) {
            orders.push_back(log(errors[i] / errors[i + 1]) / log(2.0)); // log(e_h / e_{h/2}) / log(2)
        }

        cout << (flag ? "Trapezoidal Integral:" : "Simpson Integral:") << endl;
        cout << "Integral Value\t\tError\t\t\tError Order" << endl;
        for (size_t i = 0; i < N_cases.size(); ++i) {
            cout << values[i] << "\t" << errors[i] << "\t";
            if (i < 11) cout << orders[i]; // 输出误差阶（最后一种情况没有，只需要算十一个）
            else cout << "None";
            cout << endl;
        }
        cout << endl;
        flag = !flag;
    }
    return 0;
}