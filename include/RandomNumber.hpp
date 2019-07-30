#ifndef MINI_UTILS_RANDOM_NUMBER_INC
#define MINI_UTILS_RANDOM_NUMBER_INC

#include <cstdint>

namespace MiniUtils {

// generate random numbers
class RandomNumber
{
public:
	// initialize the random number generator
	RandomNumber(int32_t s = 1)
    {
        seed(s);
    }

    // set seed of random number
    void seed(int32_t s)
    {
        if (s == 0) {
            s = 0x5EECE66DL;
        } else if (s < 0) {
            s = (s & 0x7fffffff) ^ 0x5EECE66DL;
        }

        seed_ = s;
    }

    // get seed of random number
    int32_t seed() const 
    {
        return seed_;
    }

	// return a 32-bit random integer m, 1 <= m <= 2^31-2
	int32_t random()
    {
        int32_t tmpSeed = A * ( seed_ % Q ) - R * ( seed_ / Q );

        if( tmpSeed >= 0 )
            seed_ = tmpSeed;
        else
            seed_ = tmpSeed + M;

        return seed_;
    }

    // max number of random return, equal 2^32-2
    static int32_t max() 
    {
        return M-1;
    }

	// return a real number x, 0 <= x < 1
	double frandom()
    {
        return double(random())/double(M);
    }

    // return a real number x, a <= x < b
    double randReal(double a, double b)
    {
        return a + (b-a) * frandom();
    }

    // return a int number x, a <= x <= b
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

}   // namespace MiniUtils

#endif
