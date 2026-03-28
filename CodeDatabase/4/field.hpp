#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <memory>
#include <iomanip>
#include "fieldcell.hpp"

class EntityManager;
class Hero;

class Field {
private:
    std::mt19937 engine;
    std::uniform_int_distribution<int> dice;
    std::uniform_int_distribution<int> generatorX, generatorY;
    std::uniform_real_distribution<float> prob;

public:
    int width;
    int height;
    std::vector<std::vector<FieldCell>> box;
    int x = 0;
    int y = 0;
    
    Field(EntityManager& roster, int w = 0, int h = 0);
    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;
    bool canMoveTo(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    void show(const Hero& player) const;
    void remove_dead_entities(EntityManager& roster);
};