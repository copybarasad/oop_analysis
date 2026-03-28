#pragma once
#include "Game.h"
#include <iostream>
#include <string>

class ConsoleGame {
private:
    Game game_;
    
public:
    void run();
    
private:
    void displayGameState() const;
    void displayField() const;
    void displayPlayerInfo() const;
    void displaySpells() const;
    void displayMenu() const;
    void processInput();
    void processSpellSelection();
    void processUpgradeSelection();
    char getCellSymbol(int x, int y) const;
};