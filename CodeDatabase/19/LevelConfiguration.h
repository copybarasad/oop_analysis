#pragma once
#include <unordered_map>

#include "../world/TileType.h"
#include "entity/types/EntityType.h"

class LevelConfiguration {
public:
    LevelConfiguration() = default;

    LevelConfiguration(const int width, const int height, const std::unordered_map<TileType, int> &tiles,
                       const int intelligence, const std::unordered_map<EntityType, int> &monsters)
        : world_width_(width), world_height_(height), special_tiles_(tiles),
          monsters_(std::move(monsters)),
          monster_intelligence_(intelligence) {
    }

    const std::unordered_map<TileType, int> &tiles() const { return special_tiles_; }
    const std::unordered_map<EntityType, int> &monsters() const { return monsters_; }
    int monster_intelligence() const { return monster_intelligence_; }
    int world_width() const { return world_width_; }
    int world_height() const { return world_height_; }

private:
    int world_width_;
    int world_height_;
    std::unordered_map<TileType, int> special_tiles_;
    std::unordered_map<EntityType, int> monsters_;
    int monster_intelligence_;
};
