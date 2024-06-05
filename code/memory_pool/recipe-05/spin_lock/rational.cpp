#include "rational.hpp"

MemoryPool<Rational> *Rational::stPool = 0;
MTMemoryPool<MemoryPool<Rational>, SpinLock> *Rational::memPool = 0;
