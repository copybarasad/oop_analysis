#pragma once
#include "GameState.h"
#include <vector>

class LevelTransitionState : public GameState {
private:
    std::vector<std::string> upgradeOptions;
    
public:
    LevelTransitionState();
    
    void handleInput(GameContext& context) override;
    void render(GameContext& context) override;
    
    std::string getName() const override { return "LevelTransition"; }
    
private:
    void removeHalfSpells(GameContext& context);
    void applyUpgradeAndContinue(GameContext& context, int selectedUpgrade);
};