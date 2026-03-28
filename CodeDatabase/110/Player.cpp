#include "Player.hpp"

Player::Player (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue, unsigned int pointsValue,
        std::vector<Item*> startInventory, Weapon* startWeapon, Armor* startArmor, CombatMode modeValue) :
        Entity(id, maxHealthValue, currentHealthValue, damageValue), points(pointsValue),
        inventory(startInventory), weapon(startWeapon), armor(startArmor), mode(modeValue) {}

void Player::setWeapon (Weapon* weapon) {
    setMode(weapon->getType());
    this->weapon = weapon;
}

void Player::setArmor (Armor* armor) { this->armor = armor; }

void Player::setMode (std::string modeValue) { mode =  (modeValue == "ranged" ? CombatMode::RANGED : CombatMode::CLOSE); }

void Player::setMode (CombatMode modeValue) { mode = modeValue; }

unsigned int Player::getPoints () const { return points; }

const std::vector<Item*>& Player::getInventory () const { return inventory; }

Weapon* Player::getWeapon () const { return weapon; }

Armor* Player::getArmor () const { return armor; }

CombatMode Player::getMode () const { return mode; }

void Player::addPoints (unsigned int bonusPoints) { points += bonusPoints; }

void Player::subPoints (unsigned int spentPoints) { points -= spentPoints; }

unsigned int Player::calculateDamage () const {
    unsigned int totalDamage = damage;
    if (weapon) {
        totalDamage += weapon->calculateDamage();
    }
    return totalDamage;
}

unsigned int Player::calcuateArmor () const {
    if (armor) {
        return armor->getArmorClass();
    }
    return 0;
}

bool Player::pickUpItem (Item* item) {
    if (inventory.size() >= inventory.capacity()) {
        return false;
    }
    inventory.push_back(item);
    return true;
}