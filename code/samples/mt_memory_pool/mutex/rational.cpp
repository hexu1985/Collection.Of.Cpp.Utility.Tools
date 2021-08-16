#include "rational.hpp"

mini_util::MTMemoryPool<mini_util::MemoryPool<Rational>, std::mutex> *Rational::memPool = 0;
