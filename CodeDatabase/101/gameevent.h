// gameevent.h
#ifndef GAMEEVENT_H  // ← должно быть уникальное имя
#define GAMEEVENT_H

#include <string>
#include <memory>
#include <ctime>
#include <sstream>
#include "Enum.h"


class GameEvent {
public:
    GameEvent(EventType type, LogLevel level = LogLevel::INFO);

    virtual ~GameEvent() = default;

    EventType getType() const;
    LogLevel getLevel() const;
    std::time_t getTimestamp() const;

    virtual std::string toString() const = 0;
    virtual std::string toFormattedString() const;

protected:
    EventType type_;
    LogLevel level_;
    std::time_t timestamp_;

    std::string formatTime() const;

    std::string levelToString() const;
};
#endif // GAMEEVENT_H
