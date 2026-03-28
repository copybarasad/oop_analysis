#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "input_handler.h"

class Game {
public:
    bool isGameOver() const;
public:
    Game(int rows, int cols);
    void run();
private:
    Field field_;
    int nextIdCounter_;
    ConsoleInputHandler inputHandler_;
    int createId();
};

#endif
