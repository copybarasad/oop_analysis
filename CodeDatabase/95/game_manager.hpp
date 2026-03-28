#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <string>

class Game;

class GameManager {
private:
    Game* currentGame = nullptr;
    int currentLevel = 1;
    
public:
    GameManager() = default;
    ~GameManager();
    
    Game* createNewGame(int level = 1);
    Game* loadGameFromSave(const std::string& saveName);
    Game* getCurrentGame() const;
    int getCurrentLevel() const;
    void cleanupGame();
    bool hasGame() const;
};

#endif