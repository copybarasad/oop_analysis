#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include <string>

class Game;

class GameStateManager {
private:
    Game& game;
    
public:
    explicit GameStateManager(Game& g) : game(g) {}
    void displayGameField();
    void displayPlayerStatus();
    void displaySpellList();
    bool saveGame(const std::string& filename);
    bool loadGame(const std::string& filename);
};

#endif