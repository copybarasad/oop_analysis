#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "ispell.h"
#include <vector>

class PlayerHand {
private:
    std::vector<ISpell*> spells;
    int maxSize;
    int killsForSpell;
    const int MAXSPELLS = 5;
public:
    PlayerHand(int maxHandSize = 5, int killsNeeded = 5);
    ~PlayerHand();
    
    void addRandomSpell();
    bool useSpell(int index, GameMap& map, Position& casterPos, Position& targetPos);
    void onEnemyKilled(int enemiesKilled);
    void removeHalfRandomly();
    void addSpell(ISpell* newSpell);

    int getSpellCount() const;
    ISpell* getSpell(int index) const;
};

#endif