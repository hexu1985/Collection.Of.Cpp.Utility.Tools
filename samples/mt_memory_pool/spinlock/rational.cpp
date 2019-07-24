#include "rational.hpp"

MiniUtils::MTMemoryPool<MiniUtils::MemoryPool<Rational>, MiniUtils::SpinLock> *Rational::memPool = 0;
