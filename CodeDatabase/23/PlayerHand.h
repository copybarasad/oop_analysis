#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include <vector>
#include <string>
#include <random>
#include "Spell.h"

class GameField;
class Player;

class PlayerHand {
private:
    std::vector<Spell*> spells;
    int capacity;
    std::mt19937 rng;

public:
    explicit PlayerHand(int capacity);
    ~PlayerHand();

    int size() const;
    int getCapacity() const;
    const std::vector<Spell*>& getSpells() const;

    bool addSpell(Spell* spell);
    bool addRandomSpell();
    bool castAt(GameField& field, Player& player, int index, int targetX, int targetY);
    void clear();
    void setCapacity(int newCapacity);
    void removeSpell(int index);
    std::vector<Spell*>& getSpellsMutable();
};

#endif 

