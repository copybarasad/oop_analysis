//
// Created by Artem on 22.09.2025.
//

#include "Player.h"
#include "../../GameSetup/Utils/JsonParser.h"

const LongRangeAttack& Player::getLongRangeAttack() const noexcept {
    return longRangeAttack;
}

const CloseRangeAttack& Player::getCloseRangeAttack() const noexcept {
    return closeRangeAttack;
}

bool Player::isCloseRangeAttackSelected() const noexcept {
    return this->isCloseRangeAttackSelectedFlag;
}

std::unique_ptr<Entity> Player::clone() const {
    return std::make_unique<Player>(*this);
}

long Player::getScore() const noexcept {
    return this->score;
}

SpellHand &Player::getSpellHand() noexcept {
    return this->hand;
}

void Player::changeSelectedAttackType() noexcept {
    this->isCloseRangeAttackSelectedFlag = !this->isCloseRangeAttackSelectedFlag;
}

std::string Player::serialize() {
    std::string result = "{";
    result += "xCoordinate:" + std::to_string(this->xCoordinate) + ",";
    result += "yCoordinate:" + std::to_string(this->yCoordinate) + ",";
    result += "healthPoint:" + std::to_string(this->healthPoints) + ",";
    result += "isSlowedFlag:" + std::string(this->isDisabledFlag ? "true" : "false") + ",";
    result += "stepRange:" + std::to_string(this->stepRange) + ",";
    result += "closeRangeAttack:" + this->closeRangeAttack.serialize() + ",";
    result += "longRangeAttack:" + this->longRangeAttack.serialize() + ",";
    result += "hand:" + this->hand.serialize() + ",";
    result += "isCloseRangeAttackSelectedFlag:" + std::string(this->isCloseRangeAttackSelectedFlag ? "true" : "false") + ",";
    result += "score:" + std::to_string(this->score);
    result += "}";

    return result;
}

Player* Player::deserialize(std::map<std::string, std::string> fields, SpellFactory& spellFactory) noexcept {
    int x = std::stoi(fields["xCoordinate"]);
    int y = std::stoi(fields["yCoordinate"]);
    int health = std::stoi(fields["healthPoint"]);
    bool isDisabled = fields["isDisabledFlag"] == "true";
    int step = std::stoi(fields["stepRange"]);
    bool isCloseRangeAttackSelectedFlag = fields["isCloseRangeAttackSelectedFlag"] == "true";
    long score = std::stol(fields["score"]);
    CloseRangeAttack closeRange = CloseRangeAttack::deserialize(JsonParser::parseJsonWithNestedObj(fields["closeRangeAttack"]));
    LongRangeAttack longRange = LongRangeAttack::deserialize(JsonParser::parseJsonWithNestedObj(fields["longRangeAttack"]));
    SpellHand hand = SpellHand::deserialize(JsonParser::parseJsonWithNestedObj(fields.at("hand")), spellFactory);

    return new Player(x, y, health, isDisabled, step, closeRange, longRange, isCloseRangeAttackSelectedFlag, score, hand);
}

void Player::setCloseRangeAttack(const CloseRangeAttack &closeRangeAttack) {
    Player::closeRangeAttack = closeRangeAttack;
}

void Player::setLongRangeAttack(const LongRangeAttack &longRangeAttack) {
    Player::longRangeAttack = longRangeAttack;
}

void Player::setScore(int score) noexcept {
    this->score = score;
}
