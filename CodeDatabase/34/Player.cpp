#include "Player.h"
#include "Exceptions.h"

namespace Game {

Player::Player(int initialHealth, int initialMeleeDamage, int initialRangedDamage) :
    position(0, 0),
    health(initialHealth),
    maxHealth(initialHealth),
    score(0),
    meleeDamage(initialMeleeDamage),
    rangedDamage(initialRangedDamage),
    currentCombatMode(CombatMode::Melee),
    canMoveFlag(true),
    slowedForNextTurn(false), 
    turnsInPlace(0),
    spellHand(5),
    enhancementStacks(0),
    enemiesKilledTracker(0)
{}

void Player::move(const Utils::Point& newPosition) {
    position = newPosition;
}

void Player::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) {
        health = 0;
    }
}

void Player::switchCombatMode() {
    currentCombatMode = (currentCombatMode == CombatMode::Melee) ? CombatMode::Ranged : CombatMode::Melee;
}

void Player::addScore(int points) {
    score += points;
}

int Player::getHealth() const { return health; }
int Player::getCurrentDamage() const {
    return (currentCombatMode == CombatMode::Melee) ? meleeDamage : rangedDamage;
}
const Utils::Point& Player::getPosition() const { return position; }
void Player::addTurnsInPlace(int turns) { turnsInPlace += turns; }
void Player::resetTurnsInPlace() { turnsInPlace = 0; }
int Player::getTurnsInPlace() const { return turnsInPlace; }
int Player::getScore() const { return score; }
bool Player::isAlive() const { return health > 0; }
CombatMode Player::getCombatMode() const { return currentCombatMode; }

bool Player::canMoveThisTurn() const {
    return (turnsInPlace > 0) || !slowedForNextTurn;
}

void Player::setCanMoveThisTurn(bool state) { canMoveFlag = state; }
void Player::setSlowedForNextTurn(bool state) { slowedForNextTurn = state; }
bool Player::isSlowedForNextTurn() const { return slowedForNextTurn; }
void Player::resetTurnState() { canMoveFlag = true; }

SpellHand& Player::getSpellHand() { return spellHand; }
const SpellHand& Player::getSpellHand() const { return spellHand; }
void Player::addEnhancementStack() { enhancementStacks++; }
int Player::getEnhancementStacks() const { return enhancementStacks; }
void Player::resetEnhancementStacks() { enhancementStacks = 0; }
void Player::onEnemyKilled() { enemiesKilledTracker++; }
bool Player::shouldReceiveNewSpell() const { return enemiesKilledTracker >= 2; }
void Player::resetKillTracker() { enemiesKilledTracker = 0; }

void Player::restoreHealth() {
    health = maxHealth;
}

void Player::upgrade(int choice) {
    switch (choice) {
        case 1:
            maxHealth += 20;
            health = maxHealth; // Полностью восстанавливаем при апгрейде
            std::cout << "Max health increased to " << maxHealth << "!" << std::endl;
            break;
        case 2:
            meleeDamage += 5;
            std::cout << "Melee damage increased to " << meleeDamage << "!" << std::endl;
            break;
        case 3:
            rangedDamage += 5;
            std::cout << "Ranged damage increased to " << rangedDamage << "!" << std::endl;
            break;
        default:
            std::cout << "No upgrade selected." << std::endl;
            break;
    }
}

void Player::resetScore() {
    score = 0;
}

void Player::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&position), sizeof(position));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
    os.write(reinterpret_cast<const char*>(&score), sizeof(score));
    os.write(reinterpret_cast<const char*>(&meleeDamage), sizeof(meleeDamage));
    os.write(reinterpret_cast<const char*>(&rangedDamage), sizeof(rangedDamage));
    os.write(reinterpret_cast<const char*>(&currentCombatMode), sizeof(currentCombatMode));
    os.write(reinterpret_cast<const char*>(&enhancementStacks), sizeof(enhancementStacks));
    spellHand.serialize(os);
}

void Player::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&position), sizeof(position));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
    is.read(reinterpret_cast<char*>(&score), sizeof(score));
    is.read(reinterpret_cast<char*>(&meleeDamage), sizeof(meleeDamage));
    is.read(reinterpret_cast<char*>(&rangedDamage), sizeof(rangedDamage));
    is.read(reinterpret_cast<char*>(&currentCombatMode), sizeof(currentCombatMode));
    is.read(reinterpret_cast<char*>(&enhancementStacks), sizeof(enhancementStacks));
    if(is.fail()) throw FileReadException("Failed to read player base data.");
    spellHand.deserialize(is);
}

}