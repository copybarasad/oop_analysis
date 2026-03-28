#pragma once

#include "GameObject.h"
#include <memory>
#include "Config.h"

class Wall : public GameObject {
public:
    Wall(int x, int y, char sprite = Config::Wall::SPRITE, std::string color = Config::Wall::COLOR)
        : GameObject(x, y, sprite, color, true, ObjectTag::TAG_WALL) {}

    std::unique_ptr<GameObject> clone() const override {
        return std::make_unique<Wall>(x, y, sprite, color);
    }
};
