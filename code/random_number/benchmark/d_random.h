#include <iostream>
#include <time.h>

using namespace std;

// generate random numbers
class randomNumber
{
public:
	// initialize the random number generator
	randomNumber(int s = 0);

	// return a 32-bit random integer m, 1 <= m <= 2^31-2
	int random();

	// return a 32-bit random integer m, 0 <= m <= n-1,
	// where n <= 2^31-1
	int random(int n);

	// return a real number x, 0 <= x < 1
	double frandom();

private:
	static const int A;
	static const int M;
	static const int Q;
	static const int R;

	int seed;
};

const int randomNumber::A = 48271;
const int randomNumber::M = 2147483647;
const int randomNumber::Q = M / A;
const int randomNumber::R = M % A;

randomNumber::randomNumber(int s)
{
	if (s < 0)
		s = 0;

	if (s == 0)
	{
		// get time of day in seconds since 12:00 AM,
		// January 1, 1970
		int t_time = time(NULL);

		// mix-up bits by squaring
		t_time *= t_time;
		// result can overflow. handle cases
		// > 0, < 0, = 0
		if (t_time > 0)
			s = t_time ^ 0x5EECE66DL;
		else if (t_time < 0)
			s = (t_time & 0x7fffffff) ^ 0x5EECE66DL;
		else
			s = 0x5EECE66DL;
	}

	seed = s;
}

int randomNumber::random()
{
	int tmpSeed = A * ( seed % Q ) - R * ( seed / Q );

	if( tmpSeed >= 0 )
		seed = tmpSeed;
	else
		seed = tmpSeed + M;

	return seed;
}

int randomNumber::random(int n)
{
	double fraction = double(random())/double(M);

	return int(fraction * n);
}

double randomNumber::frandom()
{
	return double(random())/double(M);
}
