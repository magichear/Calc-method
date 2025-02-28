#ifndef SERIES_HPP
#define SERIES_HPP
class Series {
private:
    double precision;            // 预设的精度成员，没用上
    int UPPER_LIMIT = 100000000; // 1_000_000 >= k >=  \dfrac{x}{e^{x * 10^{-6}} - 1} 

public:
    Series(double precision = 1e-6);
    double phi(long double x) const;
    void printResult(long double x, long double result, bool flag) const;
    /**
     *@| S(x) = \dfrac{1}{x} [ψ(x+1) + γ]  <------- γ 为欧拉-马歇罗尼常数  ψ(x) 为 digamma 函数
     *@|                                            γ ≈ 0.57721566490153286060651209
     *@| x=0   result =  \pi ^2 / 6      巴塞尔问题
     *@| x=0.5 result =  4(1-ln2)        ψ(0.5 + 1) = -γ - 2ln2 + 2
     *@| x=1   result =  1 - 1/(k+1)     望远镜
     *@| x=√2  result =                  剥蒜，不然可以调库
     *@| x= n  result =  H_n / n         调和级数 H_n = 1 + 1/2 + 1/3 + ... + 1/n，n为正整数
     */
    double calc(long double x) const; // 新增函数声明
};

#endif