#pragma once
#include "Configs.hpp"
#include <string>
#include "Entity.hpp"
#include "Hand.hpp"

// Класс игрока, наследуется от Entity
class Player : public Entity {
private:
    int score;              // Очки игрока
    bool isRangedMode;      // Режим боя (ближний/дальний)
    int meleeDamage;        // Урон в ближнем бою
    int rangedDamage;       // Урон в дальнем бою
    Hand hand;              // Рука с заклинаниями
    int nextSpellThreshold; // Порог для получения следующей карты

public:
    Player(int startHealth = Config::Settings::PLAYER_START_HEALTH,
           int startDamage = Config::Settings::PLAYER_START_DAMAGE,
           size_t handSize = Config::Settings::DEFAULT_HAND_SIZE);
        
    // Геттеры
    int getScore() const;
    bool getIsRangedMode() const;
    
    // Сеттеры для загрузки игры
    void setScore(int newScore);
    void setRangedMode(bool ranged);
    void setMeleeDamage(int damage);
    void setRangedDamage(int damage);
    
    // Методы
    void addScore(int points);
    // Переключение между ближним и дальним боем
    std::string switchCombatMode();
    
    // Атака
    int getAttackRange() const; // Получить текущую дальность атаки
    void attack(Entity* target); // Атаковать цель

    // Доступ к руке     
    Hand& getHand();
    const Hand& getHand() const;
};
