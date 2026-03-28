#include "Player.h"
#include "Spell.h"
#include "GameField.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "Enemy.h"
#include "SpellTarget.h"
#include "GameConstants.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

Player::Player()
    : Entity(GameConstants::PLAYER_START_HEALTH, GameConstants::PLAYER_DAMAGE),
    score(GameConstants::PLAYER_START_SCORE),
    mana(GameConstants::PLAYER_START_MANA),
    maxMana(GameConstants::PLAYER_MAX_MANA),
    spellHand(3) {

    std::srand(std::time(nullptr));
    auto randomSpell = createRandomStarterSpell();
    spellHand.addSpell(std::move(randomSpell));
}

std::unique_ptr<Spell> Player::createRandomStarterSpell() const {
    int randomType = std::rand() % 2;
    if (randomType == 0) {
        return std::make_unique<DirectDamageSpell>(
            "Starter Fireball",
            GameConstants::STARTER_FIREBALL_COST,
            GameConstants::STARTER_FIREBALL_RANGE,
            GameConstants::STARTER_FIREBALL_DAMAGE
        );
    }
    else {
        return std::make_unique<AreaDamageSpell>(
            "Starter Fire Storm",
            GameConstants::STARTER_FIRESTORM_COST,
            GameConstants::STARTER_FIRESTORM_RANGE,
            GameConstants::STARTER_FIRESTORM_DAMAGE
        );
    }
}

int Player::getScore() const {
    return score;
}

int Player::getMana() const {
    return mana;
}

int Player::getMaxMana() const {
    return maxMana;
}

SpellHand& Player::getSpellHand() {
    return spellHand;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

void Player::addScore(int points) {
    score += points;
}

void Player::addMana(int amount) {
    mana += amount;
    if (mana > maxMana) {
        mana = maxMana;
    }
}

bool Player::useMana(int amount) {
    if (mana >= amount) {
        mana -= amount;
        return true;
    }
    return false;
}

void Player::restoreMana() {
    mana = maxMana;
}

bool Player::castSpell(int spellIndex, int targetX, int targetY,
    GameField& field, std::vector<Enemy>& enemies) {
    std::string spellName = spellHand.getSpellName(spellIndex);
    if (spellName.empty()) {
        std::cout << "Invalid spell index!" << std::endl;
        return false;
    }

    int manaCost = spellHand.getSpellManaCost(spellIndex);
    if (!useMana(manaCost)) {
        std::cout << "Not enough mana! Required: " << manaCost
            << ", available: " << mana << std::endl;
        return false;
    }

    std::cout << "Casting " << spellName << "..." << std::endl;
    auto playerPos = field.getPlayerPosition();
    SpellTarget target(targetX, targetY, playerPos.first, playerPos.second);
    return spellHand.castSpell(spellIndex, target, field, enemies, *this);
}

void Player::setHealth(int newHealth) {
    health = newHealth;
    if (health < 0) health = 0;
}

void Player::setMana(int newMana) {
    mana = newMana;
    if (mana > maxMana) mana = maxMana;
    if (mana < 0) mana = 0;
}

void Player::setMaxMana(int newMaxMana) {
    maxMana = newMaxMana;
    if (mana > maxMana) mana = maxMana;
}

void Player::setScore(int newScore) {
    score = newScore;
    if (score < 0) score = 0;
}

void Player::clearSpellHand() {
    spellHand.clearHand();
}