#pragma once
#include <nlohmann/json.hpp>

enum class Attribute {
    Health,
    MaxHealth,
    Damage,
    RangedDamage,
    Intelligence,
    Gold
};


NLOHMANN_JSON_SERIALIZE_ENUM(Attribute, {
    {Attribute::Health, "Health"},
    {Attribute::MaxHealth, "MaxHealth"},
    {Attribute::Damage, "Damage"},
    {Attribute::RangedDamage, "RangedDamage"},
    {Attribute::Intelligence, "Intelligence"},
    {Attribute::Gold, "Gold"}
})
