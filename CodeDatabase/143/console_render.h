#pragma once

#include "game_controller.h"
#include "spell_card.h"
#include <iostream>
#include <algorithm>

class ConsoleRenderer {
public:
    void render(const GameController& game) const;
    void renderMainMenu() const;
    void renderGameOver(const GameController& game) const;
    void renderLevelUp(int level) const;
    void renderShop(const GameController& game) const;
    void renderSpells(const GameController& game) const;
    void renderTargetHelp(const GameController& game, const std::vector<Position>& validTargets, bool isArea = false) const;
    void renderMessage(const std::string& msg) const { std::cout << msg << "\n"; }
    void renderQuitPrompt() const;

private:
    void renderField(const GameController& game) const;
    void renderStats(const GameController& game) const;
};
