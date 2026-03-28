#ifndef COMBAT_TYPE_H
#define COMBAT_TYPE_H

#include <stdexcept>

enum class CombatType {
    MELEE,
    RANGED
};

// === JSON ===
#include "json.hpp"

inline void to_json(nlohmann::json& j, const CombatType& ct) {
    switch (ct) {
        case CombatType::MELEE:
            j = "MELEE";
            break;
        case CombatType::RANGED:
            j = "RANGED";
            break;
        default:
            throw std::invalid_argument("Invalid CombatType value for serialization");
    }
}

inline void from_json(const nlohmann::json& j, CombatType& ct) {
    std::string s = j.get<std::string>();
    if (s == "MELEE") {
        ct = CombatType::MELEE;
    } else if (s == "RANGED") {
        ct = CombatType::RANGED;
    } else {
        throw std::invalid_argument("Invalid CombatType string for deserialization: " + s);
    }
}

#endif