#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "OutputTerminal.h"
#include <memory>

class GameField;
class Player;
class Game; 

template<typename TOutputHandler>
class GameRenderer {
private:
    Game* game;
    std::unique_ptr<TOutputHandler> outputHandler;
    bool isRunning;
    
public:
    explicit GameRenderer(Game* gameInstance);
    ~GameRenderer() = default;

    void displayField(GameField& field);
    void displayPlayer(Player& player);
    void displaySpells(Player& player);
    void displayVictory(Game& game);
    void displayDefeat(Game& game);
    void displayFinishLevel(Game& game);
    void displayUpgradeChoice(Game& game);
};

#endif