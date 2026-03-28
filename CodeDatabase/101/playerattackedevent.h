#ifndef PLAYERATTACKEDEVENT_H
#define PLAYERATTACKEDEVENT_H
#include "gameevent.h"

class PlayerAttackedEvent : public GameEvent {
private:
    std::string target_;
    int damage_;
    int x_, y_;

public:
    PlayerAttackedEvent(const std::string& target, int damage, int x, int y);
    std::string toString() const override;
};

#endif // PLAYERATTACKEDEVENT_H
