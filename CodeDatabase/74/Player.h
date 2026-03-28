#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Types.h"
#include "Hand.h"
#include "Serialization.h"
#include <map>

class Player : public Entity {
private:
    AttackMode mode;
    int maxHealth;
    int damage;
    Coords aimPos;
    Hand hand;
    std::map<AttackMode, int> attackDamage = {
        { AttackMode::Melee, PLAYER_MELEE_DAMAGE },
        { AttackMode::Ranged, PLAYER_RANGED_DAMAGE }
    };

public:
    Player();
    Player(int health, int damage, int x, int y);

    AttackMode getAttackMode() const;
    void switchAttackMode(AttackMode newMode);

    Coords getAimPos() const;
    void moveAimPos(int dx, int dy);

    int getDamage() const;

    Hand& getHand();
    const Hand& getHand() const;

    void log() const;

    json serialize() const;
    void deserialize(const json& j);

    void restoreHealth() {
        health = maxHealth;
    }
};

#endif // PLAYER_H
