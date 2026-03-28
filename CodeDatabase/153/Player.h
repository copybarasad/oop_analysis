#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "SpellHand.h"
#include <memory>

class GameField;
class Enemy;

class Player : public Entity {
private:
    int score;
    int mana;
    int maxMana;
    SpellHand spellHand;
    std::unique_ptr<Spell> createRandomStarterSpell() const;

public:
    Player();
    int getScore() const;
    int getMana() const;
    int getMaxMana() const;
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    void addScore(int points);
    void addMana(int amount);
    bool useMana(int amount);
    void restoreMana();
    bool castSpell(int spellIndex, int targetX, int targetY,
        GameField& field, std::vector<Enemy>& enemies);

    void setHealth(int newHealth);
    void setMana(int newMana);
    void setMaxMana(int newMaxMana);
    void setScore(int newScore);
    void clearSpellHand();
};

#endif
