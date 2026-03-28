#pragma once
#include "Enemy.hpp"
#include "./Field/Cell.hpp"

class Slime : public Enemy {
    public:
        Slime(int startX, int startY, int offsetX, int offsetY, int level = 1);

};