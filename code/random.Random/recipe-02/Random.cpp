#include "Random.hpp"

namespace random_ {

static Random& get_instance() {
    static Random random;
    return random;
}

int randint() {
    auto& random = get_instance();
    return random.randint();
}

int randint(int low, int high) {
    auto& random = get_instance();
    return random.randint(low, high);
}

void seed(Random::SeedType x) {
    auto& random = get_instance();
    random.seed(x);
}

double random() {
    auto& random = get_instance();
    return random.random();
}

}   // namespace random
