#ifndef SPAWNEVENT_H
#define SPAWNEVENT_H

#include "Event.h"

class SpawnEvent : public Event {
private:
    std::string spawner;
    std::string spawned;
    std::pair<int, int> spawnPos;

public:
    SpawnEvent(std::string spawner, std::string spawned, std::pair<int, int> spawnPos);
    std::string toString() const override;
};

#endif