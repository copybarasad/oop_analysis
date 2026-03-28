#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "spell_hand.h"
#include <memory>

class Player : public Entity {
private:
    bool isMeleeMode;
    int baseDamage;
    int score;
    int mana;
    int maxMana;
    SpellHand spellHand;

public:
    Player(int hp, int dmg, size_t handSize = 5);

    // Геттеры
    bool getCombatMode() const;
    int getScore() const;
    int getMana() const;
    int getMaxMana() const;
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;

    // Сеттеры для сохранения/загрузки
    void setMana(int newMana);
    void setScore(int newScore);
    void setCombatMode(bool mode);
    void setHealth(int newHealth);

    void switchCombatMode();
    void addScore(int points);
    bool castSpell(int spellIndex, class Game& game, int targetX, int targetY);
    
    // Восстановление маны
    void restoreMana(int amount);
};

#endif