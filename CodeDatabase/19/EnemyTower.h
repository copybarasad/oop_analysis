#pragma once

#include "entity/Entity.h"

class EnemyTower : public Entity {
public:
    EnemyTower(int x, int y, int shoot_rate = 3);

    void render(const IRenderer & renderer, const World &world, int center_x, int center_y) override;
    void tick(GameLogic &logic) override;

    json serialize() const override {
        json j = Entity::serialize();
        j["rate"] = shoot_rate_;
        j["tick"] = shoot_tick_;

        return j;
    }

    bool deserialize(json data) override {
        if (!data.contains("rate") || !data.contains("tick")) {
            return false;
        }
        shoot_tick_ = data["tick"];
        shoot_rate_ = data["rate"];

        return Entity::deserialize(data);
    }

    int move_priority() const override {
        return 40;
    }
private:
    int shoot_rate_ = 0;
    int shoot_tick_ = 0;
};
