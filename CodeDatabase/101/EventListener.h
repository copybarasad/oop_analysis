#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H


#include "gameevent.h"
#include <memory>


class EventManager;

class EventListener {
public:
    virtual ~EventListener() = default;
    virtual void onEvent(const std::shared_ptr<GameEvent>& event) = 0;
    virtual LogLevel getMinLogLevel() const = 0;
private:

    inline static EventManager* instance_ = nullptr;
};

#endif // EVENTLISTENER_H
