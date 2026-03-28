#pragma once
#include "GameState.h"
#include <vector>
#include <string>

class PauseState : public GameState {
private:
    int selectedOption;
    std::vector<std::string> menuOptions;
    void saveGame(GameContext& context);
    void loadGame(GameContext& context);
    
public:
    PauseState();

    void handleInput(GameContext& context) override;
    void render(GameContext& context) override;
    
    std::string getName() const override { return "Pause"; } 
};