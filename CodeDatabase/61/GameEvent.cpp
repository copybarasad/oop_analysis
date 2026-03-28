#include "GameEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

GameEvent::GameEvent(EventType type, int turnNumber)
    : type(type), turnNumber(turnNumber) {
    timestamp = getCurrentTimestamp();
}

EventType GameEvent::getType() const {
    return type;
}

int GameEvent::getTurnNumber() const {
    return turnNumber;
}

std::string GameEvent::getTimestamp() const {
    return timestamp;
}

std::string GameEvent::getCurrentTimestamp() const {
    auto now = std::time(nullptr);
    auto* timeinfo = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// PlayerMovedEvent
PlayerMovedEvent::PlayerMovedEvent(int fromX, int fromY, int toX, int toY, int turnNumber)
    : GameEvent(EventType::PLAYER_MOVED, turnNumber),
      fromX(fromX), fromY(fromY), toX(toX), toY(toY) {
}

std::string PlayerMovedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Player moved from (" << fromX << ", " << fromY 
        << ") to (" << toX << ", " << toY << ")";
    return oss.str();
}

// PlayerDamagedEvent
PlayerDamagedEvent::PlayerDamagedEvent(int damage, int remainingHealth, int maxHealth, 
                                       const std::string& source, int turnNumber)
    : GameEvent(EventType::PLAYER_DAMAGED, turnNumber),
      damage(damage), remainingHealth(remainingHealth), maxHealth(maxHealth), source(source) {
}

std::string PlayerDamagedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Player took " << damage << " damage from " << source 
        << ". Health: " << remainingHealth << "/" << maxHealth;
    return oss.str();
}

// PlayerHealedEvent
PlayerHealedEvent::PlayerHealedEvent(int amount, int currentHealth, int maxHealth, int turnNumber)
    : GameEvent(EventType::PLAYER_HEALED, turnNumber),
      amount(amount), currentHealth(currentHealth), maxHealth(maxHealth) {
}

std::string PlayerHealedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Player healed for " << amount 
        << ". Health: " << currentHealth << "/" << maxHealth;
    return oss.str();
}

// PlayerAttackedEvent
PlayerAttackedEvent::PlayerAttackedEvent(int damage, int targetX, int targetY, 
                                         const std::string& targetType, bool killed, int turnNumber)
    : GameEvent(EventType::PLAYER_ATTACKED, turnNumber),
      damage(damage), targetX(targetX), targetY(targetY), targetType(targetType), killed(killed) {
}

std::string PlayerAttackedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Player attacked " << targetType 
        << " at (" << targetX << ", " << targetY << ") for " << damage << " damage";
    if (killed) {
        oss << ". Target killed";
    }
    return oss.str();
}

// EnemyDamagedEvent
EnemyDamagedEvent::EnemyDamagedEvent(int enemyId, int damage, int remainingHealth, 
                                      int x, int y, const std::string& source, int turnNumber)
    : GameEvent(EventType::ENEMY_DAMAGED, turnNumber),
      enemyId(enemyId), damage(damage), remainingHealth(remainingHealth), x(x), y(y), source(source) {
}

std::string EnemyDamagedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Enemy #" << enemyId << " at (" << x << ", " << y 
        << ") took " << damage << " damage from " << source 
        << ". Health: " << remainingHealth;
    return oss.str();
}

// EnemyKilledEvent
EnemyKilledEvent::EnemyKilledEvent(int enemyId, int x, int y, int scoreGained, int turnNumber)
    : GameEvent(EventType::ENEMY_KILLED, turnNumber),
      enemyId(enemyId), x(x), y(y), scoreGained(scoreGained) {
}

std::string EnemyKilledEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Enemy #" << enemyId << " at (" << x << ", " << y 
        << ") was killed. Score gained: +" << scoreGained;
    return oss.str();
}

// EnemySpawnedEvent
EnemySpawnedEvent::EnemySpawnedEvent(int enemyId, int x, int y, int health, int damage, int turnNumber)
    : GameEvent(EventType::ENEMY_SPAWNED, turnNumber),
      enemyId(enemyId), x(x), y(y), health(health), damage(damage) {
}

std::string EnemySpawnedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Enemy #" << enemyId << " spawned at (" << x << ", " << y 
        << ") with health: " << health << ", damage: " << damage;
    return oss.str();
}

// SpellCastEvent
SpellCastEvent::SpellCastEvent(const std::string& spellName, int targetX, int targetY, 
                                bool success, int turnNumber)
    : GameEvent(EventType::SPELL_CAST, turnNumber),
      spellName(spellName), targetX(targetX), targetY(targetY), success(success) {
}

std::string SpellCastEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Player cast spell '" << spellName 
        << "' at (" << targetX << ", " << targetY << "). Result: " 
        << (success ? "success" : "failed");
    return oss.str();
}

// SpellObtainedEvent
SpellObtainedEvent::SpellObtainedEvent(const std::string& spellName, int score, int turnNumber)
    : GameEvent(EventType::SPELL_OBTAINED, turnNumber),
      spellName(spellName), score(score) {
}

std::string SpellObtainedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Player obtained spell '" << spellName 
        << "' at score: " << score;
    return oss.str();
}

// TrapTriggeredEvent
TrapTriggeredEvent::TrapTriggeredEvent(int x, int y, int damage, const std::string& targetType, int turnNumber)
    : GameEvent(EventType::TRAP_TRIGGERED, turnNumber),
      x(x), y(y), damage(damage), targetType(targetType) {
}

std::string TrapTriggeredEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Trap at (" << x << ", " << y 
        << ") triggered by " << targetType << " for " << damage << " damage";
    return oss.str();
}

// AllySummonedEvent
AllySummonedEvent::AllySummonedEvent(int allyId, int x, int y, int turnNumber)
    : GameEvent(EventType::ALLY_SUMMONED, turnNumber),
      allyId(allyId), x(x), y(y) {
}

std::string AllySummonedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Ally #" << allyId << " summoned at (" << x << ", " << y << ")";
    return oss.str();
}

// LevelUpEvent
LevelUpEvent::LevelUpEvent(int newLevel, int turnNumber)
    : GameEvent(EventType::LEVEL_UP, turnNumber),
      newLevel(newLevel) {
}

std::string LevelUpEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Level up! New level: " << newLevel;
    return oss.str();
}

// ScoreChangedEvent
ScoreChangedEvent::ScoreChangedEvent(int newScore, int scoreChange, int turnNumber)
    : GameEvent(EventType::SCORE_CHANGED, turnNumber),
      newScore(newScore), scoreChange(scoreChange) {
}

std::string ScoreChangedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Score changed: " << (scoreChange >= 0 ? "+" : "") 
        << scoreChange << " = " << newScore;
    return oss.str();
}

// TurnStartedEvent
TurnStartedEvent::TurnStartedEvent(int turnNumber)
    : GameEvent(EventType::TURN_STARTED, turnNumber) {
}

std::string TurnStartedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Turn started";
    return oss.str();
}

// GameStartedEvent
GameStartedEvent::GameStartedEvent(int fieldWidth, int fieldHeight)
    : GameEvent(EventType::GAME_STARTED, 0),
      fieldWidth(fieldWidth), fieldHeight(fieldHeight) {
}

std::string GameStartedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Game Start] Game started with field size: " << fieldWidth << "x" << fieldHeight;
    return oss.str();
}

// GameEndedEvent
GameEndedEvent::GameEndedEvent(bool playerWon, int finalScore, int turnNumber)
    : GameEvent(EventType::GAME_ENDED, turnNumber),
      playerWon(playerWon), finalScore(finalScore) {
}

std::string GameEndedEvent::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turnNumber << "] Game ended. " 
        << (playerWon ? "Player won" : "Player died") 
        << " with final score: " << finalScore;
    return oss.str();
}

