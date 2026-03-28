#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>
#include <memory>
#include <ctime>

// Типы игровых событий
enum class EventType {
    PLAYER_MOVED,
    PLAYER_DAMAGED,
    PLAYER_HEALED,
    PLAYER_ATTACKED,
    ENEMY_DAMAGED,
    ENEMY_KILLED,
    ENEMY_SPAWNED,
    SPELL_CAST,
    SPELL_OBTAINED,
    TRAP_TRIGGERED,
    ALLY_SUMMONED,
    LEVEL_UP,
    SCORE_CHANGED,
    TURN_STARTED,
    GAME_STARTED,
    GAME_ENDED
};

// Базовый класс для игровых событий
class GameEvent {
public:
    GameEvent(EventType type, int turnNumber = 0);
    virtual ~GameEvent() = default;
    
    EventType getType() const;
    int getTurnNumber() const;
    std::string getTimestamp() const;
    virtual std::string toString() const = 0;
    
protected:
    EventType type;
    int turnNumber;
    std::string timestamp;
    
private:
    std::string getCurrentTimestamp() const;
};

// Событие перемещения игрока
class PlayerMovedEvent : public GameEvent {
public:
    PlayerMovedEvent(int fromX, int fromY, int toX, int toY, int turnNumber);
    std::string toString() const override;
    
private:
    int fromX, fromY, toX, toY;
};

// Событие получения урона игроком
class PlayerDamagedEvent : public GameEvent {
public:
    PlayerDamagedEvent(int damage, int remainingHealth, int maxHealth, const std::string& source, int turnNumber);
    std::string toString() const override;
    
private:
    int damage;
    int remainingHealth;
    int maxHealth;
    std::string source;
};

// Событие лечения игрока
class PlayerHealedEvent : public GameEvent {
public:
    PlayerHealedEvent(int amount, int currentHealth, int maxHealth, int turnNumber);
    std::string toString() const override;
    
private:
    int amount;
    int currentHealth;
    int maxHealth;
};

// Событие атаки игрока
class PlayerAttackedEvent : public GameEvent {
public:
    PlayerAttackedEvent(int damage, int targetX, int targetY, const std::string& targetType, bool killed, int turnNumber);
    std::string toString() const override;
    
private:
    int damage;
    int targetX, targetY;
    std::string targetType;
    bool killed;
};

// Событие получения урона врагом
class EnemyDamagedEvent : public GameEvent {
public:
    EnemyDamagedEvent(int enemyId, int damage, int remainingHealth, int x, int y, const std::string& source, int turnNumber);
    std::string toString() const override;
    
private:
    int enemyId;
    int damage;
    int remainingHealth;
    int x, y;
    std::string source;
};

// Событие убийства врага
class EnemyKilledEvent : public GameEvent {
public:
    EnemyKilledEvent(int enemyId, int x, int y, int scoreGained, int turnNumber);
    std::string toString() const override;
    
private:
    int enemyId;
    int x, y;
    int scoreGained;
};

// Событие появления врага
class EnemySpawnedEvent : public GameEvent {
public:
    EnemySpawnedEvent(int enemyId, int x, int y, int health, int damage, int turnNumber);
    std::string toString() const override;
    
private:
    int enemyId;
    int x, y;
    int health;
    int damage;
};

// Событие применения заклинания
class SpellCastEvent : public GameEvent {
public:
    SpellCastEvent(const std::string& spellName, int targetX, int targetY, bool success, int turnNumber);
    std::string toString() const override;
    
private:
    std::string spellName;
    int targetX, targetY;
    bool success;
};

// Событие получения заклинания
class SpellObtainedEvent : public GameEvent {
public:
    SpellObtainedEvent(const std::string& spellName, int score, int turnNumber);
    std::string toString() const override;
    
private:
    std::string spellName;
    int score;
};

// Событие срабатывания ловушки
class TrapTriggeredEvent : public GameEvent {
public:
    TrapTriggeredEvent(int x, int y, int damage, const std::string& targetType, int turnNumber);
    std::string toString() const override;
    
private:
    int x, y;
    int damage;
    std::string targetType;
};

// Событие призыва союзника
class AllySummonedEvent : public GameEvent {
public:
    AllySummonedEvent(int allyId, int x, int y, int turnNumber);
    std::string toString() const override;
    
private:
    int allyId;
    int x, y;
};

// Событие повышения уровня
class LevelUpEvent : public GameEvent {
public:
    LevelUpEvent(int newLevel, int turnNumber);
    std::string toString() const override;
    
private:
    int newLevel;
};

// Событие изменения счета
class ScoreChangedEvent : public GameEvent {
public:
    ScoreChangedEvent(int newScore, int scoreChange, int turnNumber);
    std::string toString() const override;
    
private:
    int newScore;
    int scoreChange;
};

// Событие начала хода
class TurnStartedEvent : public GameEvent {
public:
    TurnStartedEvent(int turnNumber);
    std::string toString() const override;
};

// Событие начала игры
class GameStartedEvent : public GameEvent {
public:
    GameStartedEvent(int fieldWidth, int fieldHeight);
    std::string toString() const override;
    
private:
    int fieldWidth, fieldHeight;
};

// Событие окончания игры
class GameEndedEvent : public GameEvent {
public:
    GameEndedEvent(bool playerWon, int finalScore, int turnNumber);
    std::string toString() const override;
    
private:
    bool playerWon;
    int finalScore;
};

#endif // GAME_EVENT_H

