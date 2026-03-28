#pragma once

#include "entity/Entity.h"

class Ally : public Entity {
public:
    Ally(int x, int y);

    void render(const IRenderer & renderer, const World &world, int center_x, int center_y) override;

    void tick(GameLogic &logic) override;

    bool is_hostile() const override {
        return false;
    }

    int move_priority() const override {
        return 10;
    }
};
