#include "level_manager.h"
#include "game.h"
#include "game_initializer.h"
#include "exceptions.h"
#include <algorithm>

void LevelManager::advanceToNextLevel(Game& game) {
    try {
        game.currentLevel++;

        game.enemies.clear();
        game.buildings.clear();
        game.towers.clear();
        game.allies.clear();

        int newW = std::min((int)game.field.getWidth() + 2, 25);
        int newH = std::min((int)game.field.getHeight() + 2, 25);
        
        if (newW > 25 || newH > 25) {
            throw GameLogicException("Level " + std::to_string(game.currentLevel) + 
                                    " field size (" + std::to_string(newW) + "x" + std::to_string(newH) + 
                                    ") exceeds maximum allowed size (25x25)");
        }
        
        game.field = GameField(newW, newH);

        int px = newW / 2;
        int py = newH / 2;
        game.player.setPosition(px, py);
        game.field.setCell(px, py, CellType::EMPTY);

        game.player.setHealth(game.player.getMaxHealth());

        if (game.spellManager) {
            game.spellManager->getHand().discardRandomHalf();
        }

        if (game.spellManager && game.spellManager->getHand().getSpellCount() == 0) {
            GameInitializer::initializeSpells(game);
        }

        int enemyCount = 3 + game.currentLevel; 
        int buildingCount = 1 + game.currentLevel / 2;
        int towerCount = 1 + game.currentLevel / 3;

        GameInitializer::spawnInitialEnemies(game, enemyCount);
        GameInitializer::spawnInitialBuildings(game, buildingCount);
        GameInitializer::spawnInitialTowers(game, towerCount);

        game.combatSystem = std::make_unique<CombatSystem>(game.player, game.enemies, game.buildings);
        game.enemyManager = std::make_unique<EnemyManager>(game.enemies, game.field, game.player, game.allies);
    } catch (const GameLogicException&) {
        throw;
    } catch (const std::exception& ex) {
        throw GameLogicException(std::string("Failed to advance to level ") + 
                                std::to_string(game.currentLevel) + ": " + ex.what());
    }
}