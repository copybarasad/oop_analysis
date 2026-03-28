#include "moneychangedevent.h"

MoneyChangedEvent::MoneyChangedEvent(int amount, int newBalance)
    : GameEvent(EventType::PLAYER_MONEY_CHANGED, LogLevel::INFO),
    amount_(amount), newBalance_(newBalance) {}

std::string MoneyChangedEvent::toString() const {
    std::ostringstream oss;
    oss << "Money changed by " << amount_
        << " (new balance: " << newBalance_ << ")";
    return oss.str();
}
