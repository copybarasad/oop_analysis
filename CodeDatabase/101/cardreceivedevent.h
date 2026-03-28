#ifndef CARDRECEIVEDEVENT_H
#define CARDRECEIVEDEVENT_H
#include "gameevent.h"

class CardReceivedEvent : public GameEvent {
private:
    std::string cardName_;

public:
    CardReceivedEvent(const std::string& cardName);

    std::string toString() const override;
};

#endif // CARDRECEIVEDEVENT_H
