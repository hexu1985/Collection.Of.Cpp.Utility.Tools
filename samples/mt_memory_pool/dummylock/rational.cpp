#include "rational.hpp"

mini_utils::MTMemoryPool<mini_utils::MemoryPool<Rational>, DummyLock> *Rational::memPool = 0;
