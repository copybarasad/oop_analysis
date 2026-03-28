//
// Created by Artem on 22.09.2025.
//

#include "CompControlledCreature.h"
#include "../../GameSetup/Utils/JsonParser.h"

int CompControlledCreature::getDamage() noexcept {
    return damage;
}

int CompControlledCreature::getChanceToDetectHostile() noexcept {
    return chanceToDetectHostile;
}

std::unique_ptr<Entity> CompControlledCreature::clone() const {
    return std::make_unique<CompControlledCreature>(*this);
}

std::string CompControlledCreature::serialize() {
    std::string res = "{";
    res += "xCoordinate:" + std::to_string(this->xCoordinate) + ",";
    res += "yCoordinate:" + std::to_string(this->yCoordinate) + ",";
    res += "healthPoint:" + std::to_string(this->healthPoints) + ",";
    res += "type:" + JsonParser::enumEntityTypeToStringType(this->type) + ",";
    res += "isDisabledFlag:" + std::string(this->isDisabledFlag ? "true" : "false") + ",";
    res += "stepRange:" + std::to_string(this->stepRange) + ",";
    res += "damage:" + std::to_string(this->damage) + ",";
    res += "chanceToDetectHostile:" + std::to_string(this->chanceToDetectHostile);
    res += "}";
    return res;
}

CompControlledCreature* CompControlledCreature::deserialize(std::map<std::string, std::string> fields) noexcept {
    int x = std::stoi(fields["xCoordinate"]);
    int y = std::stoi(fields["yCoordinate"]);
    int health = std::stoi(fields["healthPoint"]);
    EntityType entityType = JsonParser::stringTypeToEnumEntityType(fields["type"]);
    bool isDisabled = fields["isDisabledFlag"] == "true";
    int step = std::stoi(fields["stepRange"]);
    int dmg = std::stoi(fields["damage"]);
    int chance = std::stoi(fields["chanceToDetectHostile"]);

    return new CompControlledCreature(x, y, health, entityType, isDisabled, step, dmg, chance);
}