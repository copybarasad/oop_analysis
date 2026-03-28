#ifndef ENEMYATTACKEDEVENT_H
#define ENEMYATTACKEDEVENT_H

#include "gameevent.h"

class EnemyAttackedEvent : public GameEvent {
private:
    int damage_;

public:
    explicit EnemyAttackedEvent(int damage);

    std::string toString() const override;
    int getDamage() const { return damage_; }
};

#endif
