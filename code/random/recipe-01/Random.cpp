#include "Random.hpp"

/**
 * Construct with initialValue for the state.
 */
Random::Random( int initialValue ): generator(initialValue) {
}

/**
 * Return a pseudorandom int, and change the
 * internal state.
 */
int Random::randomInt( ) {
    return generator();
}

/**
 * Return a pseudorandom double in the open range 0..1
 * and change the internal state.
 */
double Random::random0_1( )
{
    return static_cast<double>( randomInt( ) ) / generator.modulus;
}

/**
 * Return an int in the closed range [low,high], and
 * change the internal state. This is a poor implementation and
 * will be biased toward some numbers, especially if the range is large.
 */
int Random::randomInt( int low, int high )
{
    double partitionSize = static_cast<double>( generator.modulus ) / ( high - low + 1 );

    return static_cast<int>( randomInt( ) / partitionSize ) + low;
}
