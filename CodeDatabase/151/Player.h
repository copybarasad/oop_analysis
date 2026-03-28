#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"
#include "PlayerHand.h"
#include <vector>

class Player {
private:
    int damage;
    int score;
    Position position;
    PlayerHand hand;
    std::vector<int> pendingUpgrades;

public:
    int health;
    int maxHealth = 100;
    int mana;
    int maxMana = 100;
    Player(int startX, int startY, int health = 100, int mana = 100, int damage = 10);

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    // Геттеры
    int getHealth() const;
    int getDamage() const;
    int getScore() const;
    Position getPosition() const;
    PlayerHand& getHand();
    int getMana() const;
    int getPendingUpgradesCount() const;

    // Сеттеры
    void setPosition(const Position& newPos);
    void addScore(int points);

    // Действия
    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const;

    // Перемещение
    void move(int dx, int dy);

    // Система заклинаний
    bool castSpell(int spellIndex, const Position& targetPos, class GameField* field);
    void addMana(int amount);
    bool canCastSpell(int spellIndex) const;
    void addUpgrade(int strength);
    int consumeUpgrades();
    void increaseDamage(int amount);
};

#endif