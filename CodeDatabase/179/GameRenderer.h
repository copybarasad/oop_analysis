#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "IGameRenderer.h"
#include "GameField.h"
#include <iostream>
#include <string>

// ConsoleRenderer реализует IGameRenderer
class ConsoleRenderer : public IGameRenderer {
public:
    void renderField(const GameField& field) override;
    void renderGameState(const GameField& field, int level, int score) override;
    void renderHelp() override;
    void renderMessage(const std::string& message) override;
    void clearScreen() override;
    
private:
    static char getCellSymbol(const Cell& cell, const Position& pos, const GameField& field);
    static std::string getHealthBar(int health, int maxHealth);
};

#endif
