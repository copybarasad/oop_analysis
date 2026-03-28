#pragma once
#include "Enemy.hpp"
#include "./Field/Cell.hpp"

class Goblin : public Enemy {
    public:
        Goblin(int startX, int startY, int offsetX, int offsetY, int level = 1);

};