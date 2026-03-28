#pragma once
#include "GameField.h"
#include "Player.h"
#include <vector>
#include <string>

class Renderer {
public:
    static void render(const GameField& field, const Player& player, int turnCount);
    static void renderGameOver(bool playerWon, int turnCount);
    static void clearScreen();

private:
    static char getCellSymbol(const Cell& cell);
    static void renderCombatLog(const std::vector<std::string>& log);
};
