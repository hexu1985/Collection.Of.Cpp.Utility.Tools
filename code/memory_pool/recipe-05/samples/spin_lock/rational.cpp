#include "rational.hpp"

mini_util::MemoryPool<Rational> *Rational::stPool = 0;
mini_util::MTMemoryPool<mini_util::MemoryPool<Rational>, mini_util::SpinLock> *Rational::memPool = 0;
