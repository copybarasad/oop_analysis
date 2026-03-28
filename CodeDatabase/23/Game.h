#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include <string>

enum class Command {
    MOVE_UP,
    MOVE_LEFT,
    MOVE_DOWN,
    MOVE_RIGHT,
    ATTACK_LEFT,
    ATTACK_UP,
    ATTACK_RIGHT,
    ATTACK_DOWN,
    SWITCH_ATTACK,
    SKIP,
    HELP,
    QUIT,
    UNKNOWN
};

class Game {
private:
    GameField field;
    bool game_running;
    
    Command parseCommand(const std::string& cmd);
    bool handlePlayerMove(int dx, int dy);
    void printHelp();
    void processTurn();
    
public:
    Game(int width, int height);
    void run();
};

#endif 
