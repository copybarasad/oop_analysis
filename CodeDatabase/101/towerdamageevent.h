#ifndef TOWERDAMAGEEVENT_H
#define TOWERDAMAGEEVENT_H

#include "gameevent.h"

class TowerDamageEvent : public GameEvent {
private:
    int damage_;

public:
    explicit TowerDamageEvent(int damage);

    std::string toString() const override;
    int getDamage() const { return damage_; }
};

#endif
