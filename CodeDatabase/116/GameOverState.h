#pragma once
#include "GameState.h"
#include <vector>
#include <string>

class GameOverState : public GameState {
private:
    bool playerWon;
    std::vector<std::string> menuOptions;
    
public:
    GameOverState(bool won = false);

    void handleInput(GameContext& context) override;
    void render(GameContext& context) override;
    
    std::string getName() const override { return "GameOver"; }
    
private:
    void restartGame(GameContext& context);
};