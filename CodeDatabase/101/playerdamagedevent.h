#ifndef PLAYERDAMAGEDEVENT_H
#define PLAYERDAMAGEDEVENT_H
#include "gameevent.h"

class PlayerDamagedEvent : public GameEvent {
private:
    int damage_;
    std::string source_;

public:
    PlayerDamagedEvent(int damage, const std::string& source);

    std::string toString() const override;
};

#endif // PLAYERDAMAGEDEVENT_H
