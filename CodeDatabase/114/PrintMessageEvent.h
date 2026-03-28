#ifndef PRINTMESSAGEEVENT_H
#define PRINTMESSAGEEVENT_H

#include "GameEvent.h"
#include <string>

class PrintMessageEvent : public GameEvent {
private:
    std::string message;

public:
    PrintMessageEvent(const std::string& msg);
    
    std::string toString() const override;
    
    const std::string& getMessage() const { return message; }
};

#endif

