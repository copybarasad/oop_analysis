#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "interfaces/IPlayerField.h"

class PlayerController {
private:
    IPlayerField* field;
public:
    PlayerController(IPlayerField* field) : field(field) {}
    
    bool isAlive();
    bool movePlayer(int newX, int newY);
    bool playerAttack(int targetX, int targetY);
    Player& getPlayer();
};

#endif
