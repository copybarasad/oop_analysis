#pragma once

#include "GameController.h"

class ConsoleView {
public:
    static void displayGameState(const GameController& game, int currentLevel = 1);
    static void displayGameOver(const GameController& game);
    static void displayInstructions();
    static void displaySpellInfo(const GameController& game);
    static void displayTargetMode(const GameController& game, const Position& cursor);
    static void clearScreen();
};
