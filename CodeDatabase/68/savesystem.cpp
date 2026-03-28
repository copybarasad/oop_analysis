#include "savesystem.h"
#include "gamemanager.h"
#include "player.h"
#include "gamefield.h"
#include "fileraii.h"
#include <iostream>
#include <fstream>

void SaveSystem::saveGameState(const GameManager& game, const std::string& filename) {
    try {
        FileRAII file(filename, std::ios::out);
        std::fstream& fs = file.get();
        
        Player* player = game.getPlayer();
        GameField* field = game.getCurrentLevelField();
        
        // Save basic info
        fs << "LEVEL: " <<game.getCurrentLevel() << "\n";
        fs << "PLAYER'S HEALTH: " << player->getHealth() << "\n";
        fs << "MANA: " << player->getMana() << "\n";
        fs << "MAX MANA: " << player->getMaxMana() << "\n";
        fs << "POINTS: " << player->getPoints() << "\n";
        fs << "POSISTION: " << player->getPosition().x << " " << player->getPosition().y << "\n";
        fs << "FIELD: " << field->getWidth() << " " << field->getHeight() << "\n";
        
        // Save towers
        auto& towers = field->getTowers();
        fs << "TOWERS REMAINING: " <<towers.size() << "\n";
        for (const auto& tower : towers) {
            fs << "TOWER'S POSITION: " << tower->getPosition().x << " " << tower->getPosition().y << "\n";
        }
        
        // Save enemies (only alive ones)
        int aliveCount = 0;
        for (const auto& enemy : game.movingEnemies) {
            if (enemy.isAlive()) aliveCount++;
        }
        fs << "ENEMIES REMAINING: " <<aliveCount << "\n";
        for (const auto& enemy : game.movingEnemies) {
            if (enemy.isAlive()) {
                fs << "ENEMY'S POSITION (X,Y) AND HEALTH: " << enemy.getPosition().x << " " 
                   << enemy.getPosition().y << " "
                   << enemy.getHealth() << "\n";
            }
        }
        
        std::cout << "Game saved: Level " << game.getCurrentLevel() 
                  << ", " << towers.size() << " towers, "
                  << aliveCount << " enemies\n";
                  
    } catch (const FileOpenException& e) {
        throw SaveGameException("Cannot save to " + filename);
    }
}

void SaveSystem::loadGameState(GameManager& game, const std::string& filename) {
    try {
        FileRAII file(filename, std::ios::in);
        std::fstream& fs = file.get();
        
        // Read basic info
        int level, health, mana, maxMana, points, playerX, playerY, width, height;
        fs >> level >> health >> mana >> maxMana >> points >> playerX >> playerY >> width >> height;
        
        // Validate
        if (fs.fail()) {
            throw CorruptedSaveException(filename, "Basic data corrupted");
        }
        
        // Reset game
        game.cleanupCurrentLevel();
        game.currentLevelNumber = level;
        
        // Create player
        game.player = std::make_unique<Player>(health, 15, Position(playerX, playerY));
        game.player->gainMana(mana);
        game.player->setMaxMana(maxMana);
        game.player->addPoints(points);
        
        // Create field
        game.currentLevel = std::make_unique<GameField>(width, height);
        game.currentLevel->placePlayer(game.player.get());
        
        // Load towers
        int towerCount;
        fs >> towerCount;
        for (int i = 0; i < towerCount; i++) {
            int x, y;
            fs >> x >> y;
            game.currentLevel->addTower(std::make_unique<EnemyTower>(Position(x, y)));
        }
        
        // Load enemies
        int enemyCount;
        fs >> enemyCount;
        game.movingEnemies.clear();
        for (int i = 0; i < enemyCount; i++) {
            int x, y, health;
            fs >> x >> y >> health;
            game.movingEnemies.emplace_back(Position(x, y), health, 5);
        }
        
        // Add initial spell if needed
        if (game.player->getHand() && game.player->getHand()->getSpellCount() == 0) {
            game.player->getHand()->gainRandomSpell();
        }
        
        std::cout << "Game loaded: Level " << level 
                  << ", HP " << health 
                  << ", Pos(" << playerX << "," << playerY << ")\n";
        
    } catch (const FileOpenException& e) {
        throw FileNotFoundException(filename);
    } catch (const std::exception& e) {
        throw LoadGameException("Corrupted save file");
    }
}