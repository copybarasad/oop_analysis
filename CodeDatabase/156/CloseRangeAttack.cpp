//
// Created by Artem on 03.10.2025.
//

#include "CloseRangeAttack.h"

std::string CloseRangeAttack::serialize() noexcept {
    std::string res = "{";
    res += "damage:" + std::to_string(this->damage) + "}";
    return res;
}

CloseRangeAttack CloseRangeAttack::deserialize(std::map<std::string, std::string> json) noexcept {
    int damage = std::stoi(json.at("damage"));
    return CloseRangeAttack(damage);
}
