#ifndef ENDLEVELEVENT_H
#define ENDLEVELEVENT_H

#include "Event.h"

class EndLevelEvent : public Event {
private:
    std::string levelName;
    bool win;
    int playerEXP;

public:
    EndLevelEvent(const std::string& levelName, bool win, int playerEXP);
    std::string toString() const override;
};

#endif