#ifndef ENEMYSPAWNEDEVENT_H
#define ENEMYSPAWNEDEVENT_H

#include "GameEvent.h"
#include <string>

class EnemySpawnedEvent : public GameEvent {
private:
    int x;
    int y;
    std::string source;

public:
    EnemySpawnedEvent(int spawnX, int spawnY, const std::string& spawnSource);
    
    std::string toString() const override;
    
    int getX() const { return x; }
    int getY() const { return y; }
    const std::string& getSource() const { return source; }
};

#endif

