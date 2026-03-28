#pragma once

#include "GameObject.h"
#include <memory>

class SlowCell : public GameObject {
public:
    SlowCell(int x, int y, char sprite = 'X', std::string color = "\033[0m", bool solid = false, ObjectTag tags = ObjectTag::TAG_SLOW_CELL)
        : GameObject(x, y, sprite, color, solid, tags) {}

    std::unique_ptr<GameObject> clone() const override {
        return std::make_unique<SlowCell>(x, y, sprite, color, solid, static_cast<ObjectTag>(tags));
    }
};
