#pragma once
#include "GameManager.h"
#include <memory>
#include <string>
#include <fstream>
//#include "ConsoleInput.h"
#include "ConsoleRender.h"
#include "FileStorage.h"
#include "Command.h"
#include "InputHandler.h"
#include "GameView.h"
#include "GameControler.h"





class Game {
private:
    int currentLevel = 1;
    int improvehealth = 1;
    int improvehand = 0;
    std::string state = "menu";
    std::shared_ptr<GameManager> gameManager;

public:
    Game();
    
    void Start();
    void GameLoop();
    void ProcessCommand(Command command, int a1, int a2);
    int GetCurrentLevel(){return currentLevel;}
    int Getimprovehealth(){return improvehealth;}
    int Getimprovehand(){return improvehand;}
    std::shared_ptr<GameManager> GEtgameManager(){return gameManager;}
private:
    void StartNewGame();
    void NextLevel();
};