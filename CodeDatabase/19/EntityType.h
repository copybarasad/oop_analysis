#pragma once
#include <nlohmann/json.hpp>

enum class EntityType {
    Player,
    Ally,
    Enemy,
    EnemyHouse,
    EnemyTower,
    Trap
};

NLOHMANN_JSON_SERIALIZE_ENUM(EntityType, {
    {EntityType::Player, "Player"},
    {EntityType::Ally, "Ally"},
    {EntityType::Enemy, "Enemy"},
    {EntityType::EnemyHouse, "EnemyHouse"},
    {EntityType::EnemyTower, "EnemyTower"},
    {EntityType::Trap, "Trap"}
});