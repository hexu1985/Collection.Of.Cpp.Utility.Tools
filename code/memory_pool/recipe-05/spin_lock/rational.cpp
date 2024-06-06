#include "rational.hpp"

MemoryPool<Rational>* Rational::stPool = nullptr;
MTMemoryPool<MemoryPool<Rational>, SpinLock>* Rational::memPool = nullptr;
