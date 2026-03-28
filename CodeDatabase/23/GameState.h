#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Player.h"
#include "GameField.h"
#include <string>
#include <vector>

class GameState {
public:
    int currentLevel;
    int playerHp;
    int playerBaseDmg;
    int playerHandCapacity;
    int playerScore;
    int fieldWidth, fieldHeight;
    std::vector<std::string> playerSpells;
    
    GameState();
    GameState(int level, Player* player, GameField* field);
    
    void updateFromGame(int level, Player* player, GameField* field);
};

#endif 

