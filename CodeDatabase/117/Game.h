#ifndef GAME_H
#define GAME_H

#include "Controllers/BehaviourSystem.h"
#include "Input/InputController.h"
#include "Input/ConsoleInputHandler.h"
#include "Controllers/LevelSystem.h"
#include "Output/GameVisualizer.h"
#include "Output/ConsoleRenderer.h"

class Game{
private:
    LevelSystem levels;
    MovementSystem movSys;
    GameState gameState;
    InputController<ConsoleInputHandler> input;
    BehaviourSystem behaviour;
    GameVisualizer<ConsoleRenderer> view;
    
public:
    void run();
    Game();
};

#endif