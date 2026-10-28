#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random {
public:
    [[nodiscard]] static int getInt(int min, int max) {
        thread_local static std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }
};

#endif // RANDOM_H
