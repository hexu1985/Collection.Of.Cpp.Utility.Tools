/**
 * @file RandomNumber.hpp
 * @brief 一个随机数生成器类
 * @author hexu_1985@sina.com
 * @version 1.0
 * @date 2019-07-30
 */
#ifndef MINI_UTILS_RANDOM_NUMBER_INC
#define MINI_UTILS_RANDOM_NUMBER_INC

#include <cstdint>

namespace mini_utils {

/**
 * @brief 随机数生成器
 */
class RandomNumber {
public:
    /**
     * @brief 初始化随机数生成器
     *
     * @param s 初始种子值
     */
	RandomNumber(int32_t s = 1)
    {
        seed(s);
    }

    /**
     * @brief 重置种子值
     *
     * @param s 种子值
     */
    void seed(int32_t s)
    {
        if (s == 0) {
            s = 0x5EECE66DL;
        } else if (s < 0) {
            s = (s & 0x7fffffff) ^ 0x5EECE66DL;
        }

        seed_ = s;
    }

    /**
     * @brief 获取当前种子值
     *
     * @return 当前种子值
     */
    int32_t seed() const 
    {
        return seed_;
    }

    /**
     * @brief 生成一个32-bit的随机整数m: 1 <= m <= 2^31-2
     *
     * @return [1, 2^31-2]区间内的随机整数
     */
	int32_t random()
    {
        int32_t tmpSeed = A * ( seed_ % Q ) - R * ( seed_ / Q );

        if( tmpSeed >= 0 )
            seed_ = tmpSeed;
        else
            seed_ = tmpSeed + M;

        return seed_;
    }

    /**
     * @brief 返回random()生成的最小随机数: 1
     *
     * @return 最小随机数
     */
    static int32_t min()
    {
        return 1;
    }

    /**
     * @brief 返回random()生成的最大随机数: 2^32-2
     *
     * @return 最大随机数
     */
    static int32_t max() 
    {
        return M-1;
    }

    /**
     * @brief 生成一个随机实数x: 0 <= x < 1
     *
     * @return [0, 1)区间内的随机实数
     */
	double frandom()
    {
        return double(random())/double(M);
    }

    /**
     * @brief 生成一个随机实数x: a <= x < b
     *
     * @param a 随机实数的最小值(包含)
     * @param b 随机实数的最大值(不包含)
     *
     * @return [a, b)区间内的随机实数
     */
    double randReal(double a, double b)
    {
        return a + (b-a) * frandom();
    }

    /**
     * @brief 生成一个随机整数m: a <= m <= b
     *
     * @param a 随机整数的最小值
     * @param b
     *
     * @return 
     */
    long randInt(long a, long b)
    {
        return a + long((b-a+1) * frandom());
    }

private:
	static const int32_t A = 48271;
	static const int32_t M = 2147483647;
	static const int32_t Q = M / A;
	static const int32_t R = M % A;

	int32_t seed_;
};

}   // namespace mini_utils

#endif
