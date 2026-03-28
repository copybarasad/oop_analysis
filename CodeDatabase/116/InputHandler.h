#pragma once

#include <iostream>
#include <limits>
#include <string>
#include "GameControl.h"
#include "StatsRenderer.h"

class InputHandler {
private:
    bool tookTurn;
public:
    InputHandler() : tookTurn(false){};

    char getCommand(const std::string& prompt = "Введите команду: ");
    void waitForEnter(const std::string& message = "Нажмите Enter для продолжения...");
    void clearInputBuffer();

    void processInput(GameControl& controller, StatsRenderer& statsRenderer, bool& gameRunning);
    void Market(GameControl& controller, StatsRenderer& statsRenderer);
    bool getTookTurn() { return tookTurn; }
};