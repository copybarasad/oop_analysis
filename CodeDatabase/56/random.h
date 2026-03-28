#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <utility>

inline std::random_device rd;
inline std::mt19937 gen(rd());

inline int random_numb(int beg, int end) {
    std::uniform_int_distribution<> dis(beg, end);
    return dis(gen);
}

#endif