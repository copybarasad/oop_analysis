#ifndef RANDOMISER_H
#define RANDOMISER_H

#include <random>

class Randomiser {
private:
    std::random_device rd;
    std::mt19937 gen;
    
    Randomiser(); 
    
    Randomiser(const Randomiser&) = delete;
    Randomiser& operator= (const Randomiser&) = delete;

public:
    static Randomiser& get_randomiser();
    
    int get_random(int min, int max);
    
    ~Randomiser() = default;
};

#endif