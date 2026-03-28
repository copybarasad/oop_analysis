#include "randomiser.h"

Randomiser::Randomiser() : gen(rd()) {};

Randomiser& Randomiser::get_randomiser() {
    static Randomiser randomiser;
    return randomiser;
};

int Randomiser::get_random(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
};