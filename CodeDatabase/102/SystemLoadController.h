#pragma once
#include "SystemLoopController.h"

class SystemLoadController
{
    SystemLoopController _controller;

    void SaveGame();

    bool LoadGame();

    int ToCommand(std::string& input);
public:
    void ToLoadingLoop();

    void PrintIntroduction();

    bool PrintSaves();
};