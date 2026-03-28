#pragma once
#include "Game.h"
#include "GameCommand.h"
#include "GameSaveManager.h"
#include "Renderer.h"
#include <iostream>

class ExtendedGame : public Game {
public:
    void ProcessGameCommand(const GameCommand& command);
    void ProcessPlayerInputPublic(char input);
    
    bool IsGameRunning() const;

    void Initialize();
    
private:
    char ConvertCommandToChar(const GameCommand& command);
};