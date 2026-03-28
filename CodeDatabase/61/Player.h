#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerHand.h"
#include <memory>
#include <string>

enum class CombatMode {
    MELEE,
    RANGED
};

class Player {
private:
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    int positionX;
    int positionY;
    CombatMode currentMode;
    bool canMoveNextTurn;
    bool isSwitchingMode;
    int slowTurnsRemaining;
    bool slowPending;
    std::unique_ptr<PlayerHand> hand;
    int enemiesKilled;
    int lastSpellScore; // Последний счет, при котором было получено заклинание
    
    void validateInvariant() const;

public:
    Player(int startX, int startY, int initialHealth = 100, 
           int meleeDmg = 20, int rangedDmg = 15);
    
    // Getters
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getScore() const;
    int getPositionX() const;
    int getPositionY() const;
    CombatMode getCombatMode() const;
    bool canMove() const;
    bool isAlive() const;
    
    // Movement and combat
    bool moveTo(int newX, int newY);
    void takeDamage(int damage);
    void addScore(int points);
    void switchCombatMode();
    void endTurn();
    void startTurn();
    
    // Utility methods
    void heal(int amount);
    void applySlowingNextTurn();
    
    // Upgrade methods
    void upgradeMaxHealth(int amount);
    void upgradeMeleeDamage(int amount);
    void upgradeRangedDamage(int amount);
    int getMeleeDamage() const;
    int getRangedDamage() const;
    
    // Spell management
    PlayerHand* getHand();
    const PlayerHand* getHand() const;
    void onEnemyKilled();
    void checkSpellReward(); // Проверка получения заклинания по очкам
    int getEnemiesKilled() const;
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
};

#endif // PLAYER_H

