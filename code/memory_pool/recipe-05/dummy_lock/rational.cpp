#include "rational.hpp"

MemoryPool<Rational> *Rational::stPool = 0;
MTMemoryPool<MemoryPool<Rational>, DummyLock> *Rational::memPool = 0;
