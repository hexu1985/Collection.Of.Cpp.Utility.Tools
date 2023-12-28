#pragma once

#include <random>

// Random class
// This code assumes 32-bit ints,
// which are standard on modern compilers.
//
// CONSTRUCTION: with (a) no initializer or (b) an integer
//     that specifies the initial state of the generator
//
// ******************PUBLIC OPERATIONS*********************
//     Return a random number according to some distribution:
// int randint()                    --> Uniform, 1 to 2^31-1
// int random()                     --> Uniform, 0 to 1
// int randint(int low, int high)   --> Uniform low..high

class Random {
public:
    Random(): Random(std::random_device{}()) {
    }

    /**
     * Construct with x for the state.
     */
    explicit Random(int x): generator(x) {
    }

    /**
     * Return a pseudorandom int, and change the
     * internal state.
     */
    int randint() {
        return generator();
    }

    /**
     * Return a pseudorandom double in the open range 0..1
     * and change the internal state.
     */
    double random() {
        return static_cast<double>(randint()) / generator.modulus;
    }

    /**
     * Return an int in the closed range [low,high], and
     * change the internal state. This is a poor implementation and
     * will be biased toward some numbers, especially if the range is large.
     */
    int randint(int low, int high) {
        double partition_size = static_cast<double>(generator.modulus) / (high - low + 1);
        return static_cast<int>(randint() / partition_size) + low;
    }

private:
    std::minstd_rand generator;
};

