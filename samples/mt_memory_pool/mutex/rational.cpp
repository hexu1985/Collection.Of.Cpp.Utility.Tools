#include "rational.hpp"

mini_utils::MTMemoryPool<mini_utils::MemoryPool<Rational>, std::mutex> *Rational::memPool = 0;
