#include "rational.hpp"

MiniUtils::MTMemoryPool<MiniUtils::MemoryPool<Rational>, std::mutex> *Rational::memPool = 0;
