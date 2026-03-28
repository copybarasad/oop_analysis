#include "Player.h"
#include <stdexcept>
#include <sstream>
#include <string>

Player::Player(int startX, int startY, int initialHealth, 
               int meleeDmg, int rangedDmg)
    : health(initialHealth), maxHealth(initialHealth), 
      meleeDamage(meleeDmg), rangedDamage(rangedDmg), 
      score(0), positionX(startX), positionY(startY),
      currentMode(CombatMode::MELEE), canMoveNextTurn(true), 
      isSwitchingMode(false), slowTurnsRemaining(0), slowPending(false),
      hand(std::make_unique<PlayerHand>(5)), enemiesKilled(0), lastSpellScore(0) {
    
    if (initialHealth <= 0 || meleeDmg <= 0 || rangedDmg <= 0) {
        throw std::invalid_argument("Health and damage values must be positive");
    }
    
    validateInvariant();
}

void Player::validateInvariant() const {
    if (health < 0 || health > maxHealth || 
        meleeDamage <= 0 || rangedDamage <= 0 ||
        score < 0) {
        throw std::runtime_error("Player invariant violated");
    }
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

int Player::getDamage() const {
    return (currentMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

int Player::getScore() const {
    return score;
}

int Player::getPositionX() const {
    return positionX;
}

int Player::getPositionY() const {
    return positionY;
}

CombatMode Player::getCombatMode() const {
    return currentMode;
}

bool Player::canMove() const {
    return (slowTurnsRemaining == 0) && !isSwitchingMode;
}

bool Player::isAlive() const {
    return health > 0;
}

bool Player::moveTo(int newX, int newY) {
    if (!canMove()) {
        return false;
    }
    
    positionX = newX;
    positionY = newY;
    return true;
}

void Player::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    
    health -= damage;
    if (health < 0) {
        health = 0;
    }
    
    validateInvariant();
}

void Player::addScore(int points) {
    if (points < 0) {
        throw std::invalid_argument("Score points cannot be negative");
    }
    
    score += points;
    checkSpellReward(); // Проверяем, нужно ли дать новое заклинание
    validateInvariant();
}

void Player::switchCombatMode() {
    if (isSwitchingMode) {
        // Complete the mode switch
        currentMode = (currentMode == CombatMode::MELEE) ? 
                     CombatMode::RANGED : CombatMode::MELEE;
        isSwitchingMode = false;
    } else {
        // Start mode switch (takes one turn)
        isSwitchingMode = true;
    }
}

void Player::endTurn() {
    if (isSwitchingMode) {
        // Complete the mode switch at the end of turn
        currentMode = (currentMode == CombatMode::MELEE) ? 
                     CombatMode::RANGED : CombatMode::MELEE;
        isSwitchingMode = false;
    }
    if (slowTurnsRemaining > 0) {
        slowTurnsRemaining--;
    }
}

void Player::heal(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Heal amount cannot be negative");
    }
    
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
    
    validateInvariant();
}

void Player::applySlowingNextTurn() {
    slowPending = true;
}

void Player::startTurn() {
    if (slowPending) {
        slowTurnsRemaining = 1;
        slowPending = false;
    }
}

PlayerHand* Player::getHand() {
    return hand.get();
}

const PlayerHand* Player::getHand() const {
    return hand.get();
}

void Player::onEnemyKilled() {
    enemiesKilled++;
}

void Player::checkSpellReward() {
    // Каждые 50 очков дают новое заклинание
    if (hand && !hand->isFull()) {
        int scoreThreshold = lastSpellScore + 50;
        if (score >= scoreThreshold) {
            hand->addRandomSpell();
            lastSpellScore = (score / 50) * 50; // Округляем до ближайшего кратного 50
        }
    }
}

int Player::getEnemiesKilled() const {
    return enemiesKilled;
}

void Player::upgradeMaxHealth(int amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Upgrade amount must be positive");
    }
    maxHealth += amount;
    health += amount; // Также увеличиваем текущее здоровье
    validateInvariant();
}

void Player::upgradeMeleeDamage(int amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Upgrade amount must be positive");
    }
    meleeDamage += amount;
    validateInvariant();
}

void Player::upgradeRangedDamage(int amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Upgrade amount must be positive");
    }
    rangedDamage += amount;
    validateInvariant();
}

int Player::getMeleeDamage() const {
    return meleeDamage;
}

int Player::getRangedDamage() const {
    return rangedDamage;
}

std::string Player::serialize() const {
    std::ostringstream oss;
    // Формат: health:maxHealth:meleeDamage:rangedDamage:score:positionX:positionY:currentMode:canMoveNextTurn:isSwitchingMode:slowTurnsRemaining:slowPending:enemiesKilled:lastSpellScore
    oss << health << ":" << maxHealth << ":" << meleeDamage << ":" << rangedDamage << ":"
        << score << ":" << positionX << ":" << positionY << ":"
        << static_cast<int>(currentMode) << ":" << (canMoveNextTurn ? 1 : 0) << ":"
        << (isSwitchingMode ? 1 : 0) << ":" << slowTurnsRemaining << ":" << (slowPending ? 1 : 0) << ":"
        << enemiesKilled << ":" << lastSpellScore << "\n";
    
    // Сериализуем руку с заклинаниями
    if (hand) {
        oss << hand->serialize();
    } else {
        oss << "NO_HAND\n";
    }
    
    return oss.str();
}

bool Player::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    
    // Читаем первую строку с основными данными
    if (!std::getline(iss, line)) return false;
    
    std::istringstream lineStream(line);
    std::string token;
    
    // health
    if (!std::getline(lineStream, token, ':')) return false;
    health = std::stoi(token);
    
    // maxHealth
    if (!std::getline(lineStream, token, ':')) return false;
    maxHealth = std::stoi(token);
    
    // meleeDamage
    if (!std::getline(lineStream, token, ':')) return false;
    meleeDamage = std::stoi(token);
    
    // rangedDamage
    if (!std::getline(lineStream, token, ':')) return false;
    rangedDamage = std::stoi(token);
    
    // score
    if (!std::getline(lineStream, token, ':')) return false;
    score = std::stoi(token);
    
    // positionX
    if (!std::getline(lineStream, token, ':')) return false;
    positionX = std::stoi(token);
    
    // positionY
    if (!std::getline(lineStream, token, ':')) return false;
    positionY = std::stoi(token);
    
    // currentMode
    if (!std::getline(lineStream, token, ':')) return false;
    currentMode = static_cast<CombatMode>(std::stoi(token));
    
    // canMoveNextTurn
    if (!std::getline(lineStream, token, ':')) return false;
    canMoveNextTurn = (std::stoi(token) == 1);
    
    // isSwitchingMode
    if (!std::getline(lineStream, token, ':')) return false;
    isSwitchingMode = (std::stoi(token) == 1);
    
    // slowTurnsRemaining
    if (!std::getline(lineStream, token, ':')) return false;
    slowTurnsRemaining = std::stoi(token);
    
    // slowPending
    if (!std::getline(lineStream, token, ':')) return false;
    slowPending = (std::stoi(token) == 1);
    
    // enemiesKilled
    if (!std::getline(lineStream, token, ':')) return false;
    enemiesKilled = std::stoi(token);
    
    // lastSpellScore
    if (!std::getline(lineStream, token)) return false;
    lastSpellScore = std::stoi(token);
    
    // Читаем данные руки
    std::string handData;
    while (std::getline(iss, line)) {
        handData += line + "\n";
    }
    
    if (handData != "NO_HAND\n" && !handData.empty()) {
        if (!hand) {
            hand = std::make_unique<PlayerHand>(5);
        }
        if (!hand->deserialize(handData)) {
            return false;
        }
    }
    
    validateInvariant();
    return true;
}

