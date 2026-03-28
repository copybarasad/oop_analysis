#include "GameEngine.h"
#include <iostream>
#include <chrono>
#include <thread>

GameEngine::GameEngine() 
    : isRunning(false) {
}

void GameEngine::run() {
    context = std::make_unique<GameContext>();
    isRunning = true;
    mainLoop(); 
    cleanup();
}

void GameEngine::stop() {
    isRunning = false;
}

void GameEngine::mainLoop() {
    while (context->isAppRunning()) {
        context->render();
        context->handleInput();
    }
}

void GameEngine::cleanup() {
    context.reset();
}