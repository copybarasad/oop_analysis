#ifndef UTILS_RANDOM_GENERATOR_H
#define UTILS_RANDOM_GENERATOR_H

#include <random>
#include <chrono> 

namespace Utils {

class RandomGenerator {
public:
    static RandomGenerator& getInstance();

    // Запрещаем копирование и присваивание для Singleton
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    void seed(unsigned int s = 0); // Установить зерно (0 для инициализации по времени)
    int getRandomInt(int min, int max);
    bool getRandomBool();

private:
    RandomGenerator(); // Приватный конструктор для Singleton
    
    std::mt19937 randomEngine;
    // std::random_device rd; // For true random seed
};

} 

#endif 
