#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "../Controllers/GameState.h"
#include <map>
#include <iostream>

class ConsoleRenderer{
private:
    void clearScreen();

public:
    void renderField(GameState& gameState);
    void renderShop(Player& player);
    void renderPlayerInfo(Player player);
    void renderHand(Hand& hand);
    void renderMessage(const std::string& message);
    void renderErrorMessage(const std::string& message);
    void renderGameState(GameState& gameState);
    void renderMainMenu();
    void renderLevelComplete();
    void renderUpgrade();
    void renderGameOver();
    void renderKeyboardSettings(std::map<char, ActionType> keyboardSettings);
};

#endif