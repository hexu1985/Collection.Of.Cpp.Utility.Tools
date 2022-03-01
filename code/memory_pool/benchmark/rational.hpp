#ifndef RATIONAL_INC
#define RATIONAL_INC

class Rational {
public:
	Rational(int a = 0, int b = 1): n(a), d(b) {}

private:
	int n;	// 分子
	int d;	// 分母
};

#endif
