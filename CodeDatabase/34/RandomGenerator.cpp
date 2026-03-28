#include "RandomGenerator.h"

namespace Utils {

RandomGenerator::RandomGenerator() {
    seed(); 
}

RandomGenerator& RandomGenerator::getInstance() {
    static RandomGenerator instance; 
    return instance;
}

void RandomGenerator::seed(unsigned int s) {
    if (s == 0) {
        // Инициализация по текущему времени для случайности
        randomEngine.seed(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    } else {
        randomEngine.seed(s);
    }
}

int RandomGenerator::getRandomInt(int min, int max) {
    if (min > max) {
        std::swap(min, max);
    }
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(randomEngine);
}

bool RandomGenerator::getRandomBool() {
    std::bernoulli_distribution distribution(0.5); 
    return distribution(randomEngine);
}

} 
