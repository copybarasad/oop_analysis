#include "Game.h"
#include "SaveManager.h"

void Game::saveToFile(const std::string& filename) const {
    SaveManager::saveGameToFile(*this, filename);
}

void Game::loadFromFile(const std::string& filename) {
    SaveManager::loadGameFromFile(*this, filename);
    
    // Логирование загрузки игры
    if (eventObserver_ != nullptr) {
        eventObserver_->onGameLoaded(filename);
    }
}
