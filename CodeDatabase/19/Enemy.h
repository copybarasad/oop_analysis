#pragma once

#include "entity/Entity.h"

class Enemy : public Entity {
public:
    Enemy(int x, int y);

    void render(const IRenderer & renderer, const World &world, int center_x, int center_y) override;

    void tick(GameLogic &logic) override;

    int move_priority() const override {
        return 20;
    }
};
