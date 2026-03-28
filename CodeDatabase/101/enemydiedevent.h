#ifndef ENEMYDIEDEVENT_H
#define ENEMYDIEDEVENT_H
#include <string>
#include "gameevent.h"


class EnemyDiedEvent : public GameEvent {
private:
    std::string enemyType_;
    int x_, y_;

public:
    EnemyDiedEvent(const std::string& enemyType, int x, int y);

    std::string toString() const override;
};

#endif // ENEMYDIEDEVENT_H
