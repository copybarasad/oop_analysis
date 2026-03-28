#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "hand.h"

class Player : public Entity {
private:

public:

    PlayerHand hand;
    AttackMode mode = MELEE;
    
    Player() : Entity(100, 0, 0, "Player", 'P', HERO, 15, 0), hand(3) {}
    void castSpell(size_t spellIndex);
    void switchMode();
    void rangedAttack(Entity* enemy);

};

#endif
