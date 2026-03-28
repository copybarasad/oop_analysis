#pragma once
#include "GameState.h"
#include <iostream>
#include <vector>

class MainMenuState : public GameState{
private:
    std::vector<std::string> options;
public:
    MainMenuState();
    ~MainMenuState() = default;
    void handleInput(GameContext& context) override;
    void render(GameContext& context) override;
    std::string getName() const { return "MainMenu"; };
};