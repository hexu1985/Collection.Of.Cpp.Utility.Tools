#include <iostream>
#include <chrono>

#include "rational.hpp"

using namespace std;

int main()
{
	Rational *array[1000];

	// 此处开始计时
	auto start = chrono::steady_clock::now();

	for (int j = 0; j < 5000; j++) {
		for (int i = 0; i < 1000; i++) {
			array[i] = new Rational(i);
		}
		for (int i = 0; i < 1000; i++) {
			delete array[i];
		}
	}

	// 此处停止计时
	auto end = chrono::steady_clock::now();

	cout << "use time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << " ms" << endl;

	return 0;
}
