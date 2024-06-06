#include "rational.hpp"

MemoryPool<Rational>* Rational::stPool = nullptr;
MTMemoryPool<MemoryPool<Rational>, DummyLock>* Rational::memPool = nullptr;
