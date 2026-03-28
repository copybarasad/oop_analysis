#include "Player.h"
#include "Spells/DamageSpell.h"
#include "Spells/AreaSpell.h"
#include "Spells/TrapSpell.h"
#include "SaveLoad.h"
#include <stdexcept>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <iostream>

Player::Player(const std::string& playerName, const Position& startPosition)
  : name(playerName),
    health(DEFAULT_HEALTH),
    maxHealth(DEFAULT_HEALTH),
    baseDamage(DEFAULT_DAMAGE),
    position(startPosition),
    combatType(CombatType::MELEE),
    score(0),
    spellHand(3),
    mana(DEFAULT_MANA),
    maxMana(DEFAULT_MAX_MANA),
    enemiesDefeated(0),
    spellPurchaseCost(DEFAULT_SPELL_COST)
{
    if (name.empty()) {
        throw std::invalid_argument("Player name cannot be empty");
    }
    if (health <= 0) {
        throw std::invalid_argument("Player health must be positive");
    }

    addRandomSpell();
}

bool Player::move(int deltaX, int deltaY) {
    Position newPosition(position.x + deltaX, position.y + deltaY);

    position = newPosition;
    return true;
}

void Player::switchCombatType() {
    if (combatType == CombatType::MELEE) {
        combatType = CombatType::RANGED;
    } else {
        combatType = CombatType::MELEE;
    }
}

int Player::getDamage() const {
    switch (combatType)
    {
    case CombatType::MELEE:
        return baseDamage * MELEE_DAMAGE_MULTIPLIER;
    case CombatType::RANGED:
        return baseDamage * RANGED_DAMAGE_MULTIPLIER;
    default:
        return baseDamage;
    }
}

void Player::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }

    health -= damage;

    if (health < 0) {
        health = 0;
    }
}

// === Spells ===
bool Player::castSpell(int spellIndex, const Position& target) {
    if (spellIndex < 0 || spellIndex >= spellHand.getSpellCount()) {
        std::cout << "Invalid spell index!\n";
        return false;
    }
    
    const ISpell* spell = spellHand.getSpell(spellIndex);
    if (!spell) {
        std::cout << "Spell not found!\n";
        return false;
    }
    
    if (mana < spell->getManaCost()) {
        std::cout << "Not enough mana! Need " << spell->getManaCost() 
                  << ", but have only " << mana << ".\n";
        return false;
    }
    
    std::cout << "Casting " << spell->getName() << " at (" 
              << target.x << ", " << target.y << ")\n";
    
    spendMana(spell->getManaCost());
    return true;
}

void Player::addRandomSpell() {
    if (spellHand.isFull()) {
        return;
    }
    
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dis(0, 2);
    
    std::unique_ptr<ISpell> spell;
    
    switch (dis(gen)) {     // damage, range, (areaSize), name, manaCost
        case 0:
            spell = std::make_unique<DamageSpell>(30, 5, "Fire Bolt", 15);
            break;
        case 1:
            spell = std::make_unique<AreaSpell>(20, 4, 2, "Fireball", 25);
            break;
        case 2:
            spell = std::make_unique<TrapSpell>(40, 4, "Bear Trap", 20);
            break;
    }
    
    if (spellHand.addSpell(std::move(spell))) {
        std::cout << "Learned new spell: " << spellHand.getSpell(spellHand.getSpellCount() - 1)->getName() << "\n";
    }
}

bool Player::canBuySpell() const {
    return score >= spellPurchaseCost && !spellHand.isFull();
}

bool Player::buySpell() {
    if (!canBuySpell()) {
        return false;
    }
    
    score -= spellPurchaseCost;
    spellPurchaseCost += 25;
    
    addRandomSpell();
    
    std::cout << "Purchased new spell! Remaining score: " << score << "\n";
    std::cout << "Next spell will cost: " << spellPurchaseCost << " points\n";
    return true;
}

void Player::restoreMana(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Mana restore amount cannot be negative");
    }
    
    mana += amount;
    if (mana > maxMana) {
        mana = maxMana;
    }
}

void Player::spendMana(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Mana spend amount cannot be negative");
    }
    
    mana -= amount;
    if (mana < 0) {
        mana = 0;
    }
}

// === Counters ===
void Player::incrementEnemiesDefeated() {
    enemiesDefeated++;
    
    restoreMana(MANA_PER_KILL);
    
    if (enemiesDefeated % 3 == 0 && !spellHand.isFull()) {
        std::cout << "You've defeated " << enemiesDefeated << " enemies! ";
        std::cout << "You can now learn a new spell.\n";
        addRandomSpell();
    }
}

// === Find Enemies ===
int Player::distanceTo(const Position& pos) const {
    return std::abs(position.x - pos.x) + std::abs(position.y - pos.y);
}

std::vector<Position> Player::findEnemiesInRange(const std::vector<Enemy>& enemies, int range) const{
    std::vector<Position> enemiesInRange;

    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && distanceTo(enemy.getPosition()) <= range) {
            enemiesInRange.push_back(enemy.getPosition());
        }
    }
    return enemiesInRange;
}

Position Player::findClosestEnemyInRange(const std::vector<Enemy>& enemies, int range) const {
    Position closestEnemy(-1, -1);
    int minDistance = range + 1;

    for (const auto& enemy: enemies) {
        if (enemy.isAlive()) {
            int dist = distanceTo(enemy.getPosition());
            if (dist <= range && dist < minDistance) {
                minDistance = dist;
                closestEnemy = enemy.getPosition();
            }
        }
    }

    return closestEnemy;
}

// === Getters ===
Position Player::getPosition() const {
    return position;
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

CombatType Player::getCombatType() const {
    return combatType;
}

std::string Player::getCombatTypeString() const {
    switch (combatType)
    {
    case CombatType::MELEE:
        return "Melee";
    case CombatType::RANGED:
        return "Ranged";
    default:
        return "Unknown";
    }
}

int Player::getScore() const {
    return score;
}

bool Player::isAlive() const {
    return health > 0;
}

std::string Player::getName() const {
    return name;
}

SpellHand& Player::getSpellHand() {
    return spellHand;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

int Player::getMana() const {
    return mana;
}

int Player::getMaxMana() const {
    return maxMana;
}

int Player::getEnemiesDefeated() const {
    return enemiesDefeated;
}

int Player::getSpellPurchaseCost() const {
    return spellPurchaseCost;
}

// === Setters ===
void Player::setPosition(const Position& newPosition) {
    position = newPosition;
}

void Player::addScore(int points) {
    if (points < 0) {
        throw std::invalid_argument("Score points cannot be negative");
    }
    score += points;
}

void Player::heal(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Heal amount cannot be negative");
    }

    health += amount;

    if (health > maxHealth) {
        health = maxHealth;
    }
}

// === Setters for Save/Load ===
void Player::setHealth(int h) {
    if (h < 0)
        h = 0;
    if (h > maxHealth)
        h = maxHealth;
    health = h;
}

void Player::setMaxHealth(int h) {
    if (h <= 0)
        return;
    maxHealth = h;
    if (health > maxHealth)
        health = maxHealth;
}

void Player::setBaseDamage(int d) {
    if (d < 0)
        return;
    baseDamage = d;
}

void Player::setMana(int m) {
    if (m < 0)
        m = 0;
    if (m > maxMana)
        m = maxMana;
    mana = m;
}

void Player::setMaxMana(int m) {
    if (m <= 0)
        return;
    maxMana = m;
    if (mana > maxMana)
        mana = maxMana;
}

void Player::setEnemiesDefeated(int e) {
    if (e < 0)
        e = 0;
    enemiesDefeated = e;
}

void Player::setSpellPurchaseCost(int c) {
    if (c < 0)
        c = DEFAULT_SPELL_COST;
    spellPurchaseCost = c;
}

void Player::setScore(int s) {
    if (s < 0)
        s = 0;
    score = s;
}

void Player::setCombatType(CombatType ct) {
    combatType = ct;
}

int Player::getBaseDamage() const {
    return baseDamage;
}

void Player::upgradeMaxHealth(int amount) {
    if (amount <= 0)
        return;
        
    maxHealth += amount;
    if (health < maxHealth) {
        health = maxHealth;
    }
}

void Player::upgradeBaseDamage(int amount) {
    if (amount <= 0) return;
    baseDamage += amount;
}

void Player::upgradeMaxMana(int amount) {
    if (amount <= 0)
        return;

    maxMana += amount;
    if (mana < maxMana) {
        mana = maxMana;
    }
}

// === loadState ===
void Player::loadState(const nlohmann::json& j) {
    setHealth(j.at("health").get<int>());
    setMaxHealth(j.at("maxHealth").get<int>());
    setBaseDamage(j.at("baseDamage").get<int>());
    setPosition(j.at("position").get<Position>());
    setCombatType(j.at("combatType").get<CombatType>());
    setScore(j.at("score").get<int>());

    // SpellHand
    SpellHand new_spell_hand;
    j.at("spellHand").get_to(new_spell_hand);
    spellHand = std::move(new_spell_hand);

    setMana(j.at("mana").get<int>());
    setMaxMana(j.at("maxMana").get<int>());
    setEnemiesDefeated(j.at("enemiesDefeated").get<int>());
    setSpellPurchaseCost(j.at("spellPurchaseCost").get<int>());
}