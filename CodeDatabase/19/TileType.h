#pragma once
#include <nlohmann/json.hpp>

enum class TileType {
    Ground,
    Mud,
    Ice,
    Lava
};

NLOHMANN_JSON_SERIALIZE_ENUM(TileType, {
    {TileType::Ground, "Ground"},
    {TileType::Mud, "Mud"},
    {TileType::Ice, "Ice"},
    {TileType::Lava, "Lava"}
});