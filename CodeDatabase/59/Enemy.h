#pragma once

#include "Being.h"
#include "json.hpp"
using json = nlohmann::json;
class Enemy : public Being {
public:
    Enemy(int damage, Coordinates position, int maxHealth = 20);
    ~Enemy() override = default;

    json toJson() const;
    void fromJson(const json& j);
};