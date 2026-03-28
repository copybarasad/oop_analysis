#pragma once

#include "entity/Entity.h"

class Trap : public Entity {
public:
    Trap(int x, int y, bool hostile = false, int damage = 1);

    void render(const IRenderer & renderer, const World &world, int center_x, int center_y) override;

    bool is_hostile() const override;

    json serialize() const override {
        json data = Entity::serialize();
        data["hostile"] = hostile_;

        return data;
    }

    bool deserialize(json data) override {
        if (!data.contains("hostile")) return false;
        hostile_ = data["hostile"];
        return Entity::deserialize(data);
    }

    int move_priority() const override {
        return 0;
    }

private:
    bool hostile_;
};
