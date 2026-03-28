#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Hand.h"
#include <memory>
#include <iostream>

class Enemy;

class Player : public Character {
private:
    int points;
    int combatType;
    int combatPower;
    int radius;
    Hand hand;
    int damageBonus;

public:
    Player(std::string name, Field* field, int startX, int startY, int handSize);
    
    // Движение
    bool move(int dx, int dy);
    
    // Боевая система
    void switchCombatMode();
    bool attack(Enemy& target);
    
    // Система очков и заклинаний
    int getPoints() const;
    void addPoints(int amount);
    Hand& getHand();
    const Hand& getHand() const;  // Добавили const версию
    void buyRandomSpell();
    void addRandomSpell();
    int getCombatPower() const;
    
    // Позиционирование
    void resetPosition(int newX, int newY);
    
    // Сохранение/загрузка
    void save(std::ostream& os) const;
    void load(std::istream& is, Field* newField);
    
    // Прокачка
    void upgradeMaxHealth(int amount);
    void upgradeDamage(int amount);
    
    // Геттеры для рендерера (const версии)
    std::string getName() const { return name; }
    int getLives() const { return lives; }
    int getMaxLives() const { return maxLives; }
    int getX() const { return x; }
    int getY() const { return y; }
    
    // Необходимые методы для Game
    void setField(Field* newField);
    void restoreHealth();
};

#endif // PLAYER_H