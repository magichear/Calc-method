#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;
using Value = long double;

/**
 * @brief 高斯列主元消元法求解器
 */
class GaussEliminationWithPartialPivoting {
private:
    vector<vector<Value>> AA;
    vector<Value> bb;
    vector<Value> x;
    int n;


    /**
     * @brief 参考课本85页伪代码实现的求解算法，更正了两处错误并优化了两处逻辑
     * @param A 系数矩阵
     * @param b 常数项向量
     */
    void __calc(vector<vector<Value>> A, vector<Value> b) {
        for (int i = 0; i < n-1; ++i) {
            int k = i;
            for (int j = i+1; j < n; ++j) {
                if (abs(A[j][i]) > abs(A[k][i])) k = j; // 课本伪代码此处有错误（较明显）
            }

            // 主元并非当前行，直接调库交换整行，不用一个个换
            if (i != k) {
                swap(A[k], A[i]);
                swap(b[k], b[i]);
            }
            if (abs(A[i][i]) < 1e-25) { // 必须非零，不过影响不大
                cout << "无法求解" << endl;
                return;
            }
    
            for (int j = i+1; j < n; ++j) {
                Value factor = A[j][i] / A[i][i]; // 课本伪代码此处有错误
                for (int l = i+1; l < n; ++l) {
                    A[j][l] -= factor * A[i][l];
                }
                b[j] -= factor * b[i];
            }
        }
    
        // 这里可以直接求对X操作求解，课本那样处理b再返回多余且容易混淆
        for (int i = n-1; i >= 0; --i) {
            x[i] = b[i];
            for (int j = i+1; j < n; ++j) {
                x[i]  -= A[i][j] * x[j];
            }
            x[i] /= A[i][i];
        }
    }

public:
    GaussEliminationWithPartialPivoting(vector<vector<Value>> A, vector<Value> b) 
        : AA(A), bb(b), n(A.size()) {
        x.resize(n, 0.0);
    }
    /**
     * @brief 检查每行的和是否与b相同
     */
    void check() {
        cout << "\nCheck:" << endl;
        for (int i = 0; i < n; ++i) {
            Value sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += AA[i][j] * x[j];
            }
            cout << "Row " << setw(2) << left << (i + 1) << ": " 
                 << setw(20) << fixed << setprecision(12) << sum 
                 << "Real:" << setw(20) << bb[i] << endl;
        }
        cout << "\n\n" << endl;
    }
    
    /**
     * @brief 求解并打印结果
     */
    void print() {
        __calc(AA, bb);
        cout << "Solution:" << endl;
        cout << fixed << setprecision(15);
        for (int i = 0; i < n; ++i) {
            cout << left << "x" << setw(2) << (i + 1) << " = " << setw(18) << x[i];
            if (i != n - 1) cout << ", ";
            if ((i + 1) % 3 == 0) cout << endl;
        }
        cout << endl;
    }
    /**
     * @brief 允许更换求解目标
     */
    void change(const vector<vector<Value>>& A, const vector<Value>& b) {
        if (A.size() != b.size()) {
            cout << "[ERROR] 系数矩阵和常数项向量的维度不匹配" << endl;
            return;
        }
        if (A.size() != A[0].size()) {
            cout << "[ERROR] 仅支持方阵的求解" << endl;
            return;
        }
        AA = A;
        bb = b;
        n = A.size();
        x.resize(n, 0.0);
        cout << "[INFO] 已成功更换系数矩阵和常数项向量" << endl;
    }
};

int main() {
    vector<vector<Value>> A = {
        { 31, -13,   0,   0,   0, -10,   0,   0,   0},
        {-13,  35,  -9,   0, -11,   0,   0,   0,   0},
        {  0,  -9,  31, -10,   0,   0,   0,   0,   0},
        {  0,   0, -10,  79, -30,   0,   0,   0,  -9},
        {  0,   0,   0, -30,  57,  -7,   0,  -5,   0},
        {  0,   0,   0,   0,  -7,  47, -30,   0,   0},
        {  0,   0,   0,   0,   0, -30,  41,   0,   0},
        {  0,   0,   0,   0,  -5,   0,   0,  27,  -2},
        {  0,   0,   0,  -9,   0,   0,   0,  -2,  29}
    };

    vector<Value> b = {-15, 27, -23, 0, -20, 12, -7, 7, 10};

    GaussEliminationWithPartialPivoting solver(A, b);
    solver.print();
    //solver.check();

    return 0;
}