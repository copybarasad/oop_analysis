#include "game_manager.hpp"
#include "game.hpp"

GameManager::~GameManager() {
    cleanupGame();
}

Game* GameManager::createNewGame(int level) {
    cleanupGame();
    currentLevel = level;
    currentGame = new Game(level);
    return currentGame;
}

Game* GameManager::loadGameFromSave(const std::string& saveName) {
    cleanupGame();
    currentGame = new Game();
    return currentGame;
}

Game* GameManager::getCurrentGame() const {
    return currentGame;
}

int GameManager::getCurrentLevel() const {
    return currentLevel;
}

void GameManager::cleanupGame() {
    if (currentGame) {
        delete currentGame;
        currentGame = nullptr;
    }
}

bool GameManager::hasGame() const {
    return currentGame != nullptr;
}