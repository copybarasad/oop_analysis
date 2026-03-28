#pragma once

#include "entity/Entity.h"

class EnemyHouse : public Entity {
public:
    EnemyHouse(int x, int y, int spawn_rate = 3);

    void render(const IRenderer & renderer, const World &world, int center_x, int center_y) override;

    void tick(GameLogic &logic) override;

    json serialize() const override {
        json j = Entity::serialize();
        j["rate"] = spawn_rate_;
        j["tick"] = spawn_tick_;

        return j;
    }

    bool deserialize(json data) override {
        if (!data.contains("rate") || !data.contains("tick")) {
            return false;
        }
        spawn_tick_ = data["tick"];
        spawn_rate_ = data["rate"];

        return Entity::deserialize(data);
    }

    int move_priority() const override {
        return 30;
    }

private:
    int spawn_rate_ = 0;
    int spawn_tick_ = 0;
};
