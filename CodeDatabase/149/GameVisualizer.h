#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "GameEngine.h"

template<typename Renderer>
class GameVisualizer {
private:
    Renderer renderer;
    GameEngine* gameEngine;

public:
    GameVisualizer(GameEngine* engine);
    
    void displayGame(int currentLevel);
    
    void onGameStateChanged(int currentLevel);
};

#include "GameVisualizer.tpp"

#endif