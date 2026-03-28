//
// Created by bob on 10/4/25.
//

#include "Player.h"

#define DEFAULT_POINTS 100

Player::Player(std::shared_ptr<Hand> hand, const int &closeCombatDamage, const int &rangedCombatDamage,
    const Position & position, const int & health)
    : hand(hand), mode(FightMode::CloseCombat), points(DEFAULT_POINTS), Character(closeCombatDamage, position, health),
    closeCombatDamage(validateDamage(closeCombatDamage)), rangedCombatDamage(validateDamage(rangedCombatDamage)), skipNextStep(false), healthMax(health) {}

Player::Player(const std::size_t &points, const FightMode &mode, const bool &skipNextStep, const int &closeCombatDamage,
    const int &rangeCombatDamage, std::shared_ptr<Hand> hand, const int &healthMax, const Position &position)
    : Character(closeCombatDamage, position, healthMax), points(points), mode(mode), skipNextStep(skipNextStep), closeCombatDamage(closeCombatDamage), rangedCombatDamage(rangeCombatDamage),
    hand(hand), healthMax(healthMax){

    if (mode == FightMode::CloseCombat) {
        this->setDamage(closeCombatDamage);
    }
    else {
        this->setDamage(rangeCombatDamage);
    }
}


void Player::setRangedCombatDamage(const int &rangedCombatDamage) {
    this->rangedCombatDamage = validateDamage(rangedCombatDamage);
}

void Player::setCloseCombatDamage(const int &closeCombatDamage) {
    this->closeCombatDamage = validateDamage(closeCombatDamage);
}


void Player::switchFightMode() {
    mode = (mode == FightMode::CloseCombat)? FightMode::RangedCombat: FightMode::CloseCombat;
}

int Player::getScore() const { return this->points; }

FightMode Player::getFightMode() const { return this->mode; }

bool Player::isSkipStep() const { return this->skipNextStep; }

void Player::setSkipStep(const bool &isSkip) { this->skipNextStep = isSkip; }

void Player::addPoints(const int &points) { this->points += points; }

std::shared_ptr<Hand> Player::getHand() {
    return this->hand;
}

void Player::restoreHealth() {
    this->health = healthMax;
}

int Player::getCloseDamage() const {
    return this->closeCombatDamage;
}

int Player::getRangeDamage() const {
    return this->rangedCombatDamage;
}

int Player::getMaxHealth() const {
    return this->healthMax;
}

void Player::save(Saver &saver) const {
    saver.save(points);

    int modeInt = static_cast<int>(mode);
    saver.save(modeInt);

    saver.save(skipNextStep);
    saver.save(closeCombatDamage);
    saver.save(rangedCombatDamage);
    hand->save(saver);
    saver.save(healthMax);
    saver.save(health);
    saver.save(position);
}


void Player::load(Reader &reader) {
    reader.read(points);

    int modeInt;
    reader.read(modeInt);
    mode = static_cast<FightMode>(modeInt);

    reader.read(skipNextStep);
    reader.read(closeCombatDamage);
    reader.read(rangedCombatDamage);
    hand = std::shared_ptr<Hand>(new Hand);
    hand->load(reader);
    reader.read(healthMax);
    reader.read(health);
    reader.read(position);
}





