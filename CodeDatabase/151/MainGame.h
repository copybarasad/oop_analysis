#pragma once
#ifndef MAINGAME_H
#define MAINGAME_H

#include "GameField.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include "InputHandler.h"
#include "Renderer.h"

class MainGame {
private:
    GameField* field = nullptr;
    Player* player = nullptr;           
    bool running = true;
    int currentLevel = 1;

    ConsoleInputHandler inputHandler;
    ConsoleRenderer renderer;
    GameController<ConsoleInputHandler> controller;
    GameVisualizer<ConsoleRenderer> visualizer;

    void saveGame();
    void levelUp();
    void askRestartOrQuit();
    void initLevel(int width, int height);

public:
    MainGame();
    ~MainGame();

    void start();       
    void gameLoop();    
};

#endif