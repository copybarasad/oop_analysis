#ifndef RPG_RANDOM_H_
#define RPG_RANDOM_H_

#include <random>

class Random {
public:
  static std::mt19937 rng;

  static int getInt(int minInclusive, int maxInclusive) {
    if (maxInclusive < minInclusive) {
      std::swap(minInclusive, maxInclusive);
    }
    std::uniform_int_distribution<int> dist(minInclusive, maxInclusive);
    return dist(rng);
  }
};

#endif // RPG_RANDOM_H_