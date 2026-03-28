#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "player.h"
#include "game_field.h"
#include "game_enums.h"

class PlayerController {
private:
    Player& player;
    GameField& field;
    
    Position getDirectionVector(Direction direction) const;
    
public:
    PlayerController(Player& player, GameField& field);
    
    bool movePlayer(Direction direction);
    void switchCombatMode();
    const Player& getPlayer() const;
};

#endif
