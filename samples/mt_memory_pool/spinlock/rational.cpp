#include "rational.hpp"

mini_utils::MTMemoryPool<mini_utils::MemoryPool<Rational>, mini_utils::SpinLock> *Rational::memPool = 0;
