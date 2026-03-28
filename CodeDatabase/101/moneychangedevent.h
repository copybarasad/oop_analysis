#ifndef MONEYCHANGEDEVENT_H
#define MONEYCHANGEDEVENT_H
#include <string>
#include "Enum.h"
#include "gameevent.h"

class MoneyChangedEvent : public GameEvent {
private:
    int amount_;
    int newBalance_;

public:
    MoneyChangedEvent(int amount, int newBalance);

    std::string toString() const override;
};

#endif // MONEYCHANGEDEVENT_H
