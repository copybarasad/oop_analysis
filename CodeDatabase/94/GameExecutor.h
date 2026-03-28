#ifndef GAMEEXECUTOR_H
#define GAMEEXECUTOR_H

#include "Command.h"
#include "Game.h"

// Исполнитель команд (прослойка между командами и игрой)
class GameExecutor {
private:
    Game& game;
    
public:
    explicit GameExecutor(Game& g) : game(g) {}
    
    // Выполнение команды путем делегирования игре
    bool execute(const Command& command) {
        return game.execute(command);
    }
};

#endif