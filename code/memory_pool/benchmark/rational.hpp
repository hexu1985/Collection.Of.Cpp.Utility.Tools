#pragma once

class Rational {
public:
	Rational(int a = 0, int b = 1): n(a), d(b) {}

private:
	int n;	// Numerator
	int d;	// Denominator
};

