#include "rational.hpp"

MiniUtils::MTMemoryPool<MiniUtils::MemoryPool<Rational>, DummyLock> *Rational::memPool = 0;
