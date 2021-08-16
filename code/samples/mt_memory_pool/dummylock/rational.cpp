#include "rational.hpp"

mini_util::MTMemoryPool<mini_util::MemoryPool<Rational>, DummyLock> *Rational::memPool = 0;
