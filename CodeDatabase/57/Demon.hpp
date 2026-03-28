#pragma once
#include "Enemy.hpp"
#include "./Field/Cell.hpp"

class Demon : public Enemy {
    public:
        Demon(int startX, int startY, int offsetX, int offsetY, int level = 1);

};