//
// Created by Artem on 03.10.2025.
//

#include "LongRangeAttack.h"

int LongRangeAttack::getRange() const noexcept {
    return this->range;
}

std::string LongRangeAttack::serialize() noexcept {
    std::string res = "{";
    res += "damage:";
    res += std::to_string(this->damage) + ",";
    res += "range:";
    res += std::to_string(this->range);
    res += "}";
    return res;
}

LongRangeAttack LongRangeAttack::deserialize(std::map<std::string, std::string> json) noexcept {
    int damage = std::stoi(json.at("damage"));
    int range = std::stoi(json.at("range"));
    return LongRangeAttack(damage, range);
}
