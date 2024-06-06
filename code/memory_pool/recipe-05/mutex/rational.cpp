#include "rational.hpp"

MemoryPool<Rational>* Rational::stPool = 0;
MTMemoryPool<MemoryPool<Rational>, std::mutex>* Rational::memPool = 0;
