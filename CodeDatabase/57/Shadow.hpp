#pragma once
#include "Enemy.hpp"
#include "./Field/Cell.hpp"

class Shadow : public Enemy {
    public:
        Shadow(int startX, int startY, int offsetX, int offsetY, int level = 1);

};