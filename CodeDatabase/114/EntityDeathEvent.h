#ifndef ENTITYDEATHEVENT_H
#define ENTITYDEATHEVENT_H

#include "GameEvent.h"
#include <string>

class EntityDeathEvent : public GameEvent {
private:
    std::string entityName;
    int x;
    int y;

public:
    EntityDeathEvent(const std::string& name, int deathX, int deathY);
    
    std::string toString() const override;
    
    const std::string& getEntityName() const { return entityName; }
    int getX() const { return x; }
    int getY() const { return y; }
};

#endif

