#ifndef ENEMYDAMAGEEVENT_H
#define ENEMYDAMAGEEVENT_H

#include "gameevent.h"

class EnemyDamageEvent : public GameEvent {
private:
    int damage_;

public:
    explicit EnemyDamageEvent(int damage);

    std::string toString() const override;
    int getDamage() const { return damage_; }
};

#endif
