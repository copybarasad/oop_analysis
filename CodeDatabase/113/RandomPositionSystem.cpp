#include "RandomPositionSystem.h"

// Инициализация статических членов
std::random_device RandomPositionSystem::m_rd;
std::mt19937 RandomPositionSystem::m_gen(RandomPositionSystem::m_rd());

std::pair<int, int> RandomPositionSystem::selectRandomPosition(const std::vector<std::pair<int, int>>& positions) {
    if (positions.empty()) {
        return { -1, -1 };
    }

    std::uniform_int_distribution<> dist(0, static_cast<int>(positions.size() - 1));
    return positions[dist(m_gen)];
}

void RandomPositionSystem::initializeRandomGenerator() {
    // Инициализация уже выполнена в статических переменных
}