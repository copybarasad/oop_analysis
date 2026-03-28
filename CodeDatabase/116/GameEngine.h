#pragma once
#include "GameContext.h"
#include <memory>

class GameEngine {
private:
    std::unique_ptr<GameContext> context;
    bool isRunning;
    
public:
    GameEngine();
    ~GameEngine() = default;
    
    void run();
    void stop();
    
private:
    void mainLoop();
    void cleanup();
};