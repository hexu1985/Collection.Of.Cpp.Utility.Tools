#include "rational.hpp"

mini_util::MemoryPool<Rational> *Rational::stPool = 0;
mini_util::MTMemoryPool<mini_util::MemoryPool<Rational>, std::mutex> *Rational::memPool = 0;