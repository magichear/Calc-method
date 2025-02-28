#include <cmath>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "Series.hpp"

using namespace std;

int main() {
    long double xArrays[] = {0.0, 0.5, 1.0, sqrt(2), 10.0, 100.0, 300.0};
    int num_values = sizeof(xArrays) / sizeof(xArrays[0]);
    Series series;

    vector<thread> threads;
    mutex mtx;
    condition_variable cv;
    int next_index = 0;
    bool append = false;

    auto worker = [&](int index, bool flag) {
        long double x = xArrays[index];
        long double result = series.calc(x);     // <---------- 计算函数入口，其它就性能优化之类的，一开始爆算太慢

        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&] { return index == next_index; });
        series.printResult(x, result, flag);
        next_index++;
        cv.notify_all();
    };

    for (int i = 0; i < num_values; ++i) {
        threads.emplace_back(worker, i, append);    // 传参构造，push_back 是拷贝（传一个已经构造好的进去）构造
        append = true;
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
