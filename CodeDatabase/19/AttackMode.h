#pragma once
#include <nlohmann/json.hpp>

enum class AttackMode {
    Melee,
    Ranged
};
NLOHMANN_JSON_SERIALIZE_ENUM(AttackMode, {
    {AttackMode::Melee, "Melee"},
    {AttackMode::Ranged, "Ranged"}
});