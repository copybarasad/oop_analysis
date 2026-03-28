#pragma once
#include <string>
#include <utility>
#include "enums.hpp"

inline std::string EntityTypeToString(EntityType type) {
    switch (type) {
        case EntityType::Player:        return "Player";
        case EntityType::Enemy:         return "Enemy";
        case EntityType::EnemyTower:    return "Tower";
        case EntityType::EnemyBuilding: return "Building";
        case EntityType::Ally:          return "Ally";
        case EntityType::Summon:        return "Summon";
        case EntityType::Trap:          return "Trap";
        default:                        return "Unknown";
    }
}

struct EntitySnapshot {
    int id;
    EntityType type;
    int health;
    int damage;
    std::pair<int,int> pos;
    EntitySnapshot(int id, EntityType type, int health, int damage, std::pair<int,int> position)
        : id(id),
          type(type),
          health(health),
          damage(damage),
          pos(position)
    {}
    std::string toString() const {
        return 
            EntityTypeToString(type) +
            "(id=" + std::to_string(id) +
            ", hp=" + std::to_string(health) +
            ", dmg=" + std::to_string(damage) +
            ", pos=(" + std::to_string(pos.first) +
            "," + std::to_string(pos.second) + "))";
    }
};
