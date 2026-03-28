#ifndef RANDOMPOSITIONSYSTEM_H
#define RANDOMPOSITIONSYSTEM_H

#include <vector>
#include <utility>
#include <random>

class RandomPositionSystem {
private:
    static std::random_device m_rd;
    static std::mt19937 m_gen;

public:
    static std::pair<int, int> selectRandomPosition(const std::vector<std::pair<int, int>>& positions);
    static void initializeRandomGenerator();
};

#endif // RANDOMPOSITIONSYSTEM_H