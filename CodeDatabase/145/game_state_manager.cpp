#include "game_state_manager.h"
#include "game.h"
#include "save_manager.h"
#include <iostream>

void GameStateManager::displayGameField() {
    const auto& traps = game.field.getTraps();
    game.renderer->displayField(game.field, game.player, game.enemies, game.buildings, 
                                game.towers, game.allies, traps);
}

void GameStateManager::displayPlayerStatus() {
    game.renderer->displayPlayerStatus(game.player);
}

void GameStateManager::displaySpellList() {
    const auto& spells = game.spellManager->getHand().getSpells();
    std::cout << "Spells in hand (" << spells.size() << "/" << game.spellManager->getHand().getMaxSize() << "): ";
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << "[" << i << "]" << spells[i]->getName();
        if (i < spells.size() - 1) std::cout << ", ";
    }
    if (game.spellManager->getEnhancementLevel() > 0) {
        std::cout << " | Enhancements: " << game.spellManager->getEnhancementLevel();
    }
    std::cout << "\n";
}

bool GameStateManager::saveGame(const std::string& filename) {
    try {
        SaveManager::saveGameToFile(game, filename);
        game.setLastError("");
        return true;
    } catch (const std::exception& ex) {
        game.setLastError(ex.what());
        return false;
    }
}

bool GameStateManager::loadGame(const std::string& filename) {
    try {
        SaveManager::loadGameFromFile(game, filename);
        game.setLastError("");
        return true;
    } catch (const std::exception& ex) {
        game.setLastError(ex.what());
        return false;
    }
}