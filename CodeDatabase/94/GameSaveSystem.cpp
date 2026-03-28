#include "GameSaveSystem.h"
#include "LevelGenerator.h"
#include "HealSpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <sstream>
#include <iostream>

bool GameSaveSystem::saveGame(const GameController& game, int currentLevel, int playerMaxHealth) {
    try {
        SaveFile file(filename, std::ios::out);
        auto& fs = file.get();
        
        fs << "DUNGEON_CRAWLER_SAVE_V2\n";
        fs << currentLevel << "\n";
        fs << playerMaxHealth << "\n";
        
        const Player& player = game.getPlayer();
        const GameField& field = game.getField();
        
        fs << player.getScore() << "\n";
        fs << player.getHealth() << "\n";
        fs << player.getMaxHealth() << "\n";
        fs << static_cast<int>(player.getCombatMode()) << "\n";
        
        Position playerPos = field.getPlayerPosition();
        fs << playerPos.x() << "\n" << playerPos.y() << "\n";
        
        fs << player.getHand().getSpellCount() << "\n";
        for (int i = 0; i < player.getHand().getSpellCount(); ++i) {
            const Spell* spell = player.getHand().getSpell(i);
            if (spell) {
                fs << spell->getName() << "\n";
            }
        }
        
        auto enemies = game.getEnemiesInfo();
        fs << enemies.size() << "\n";
        for (const auto& enemy : enemies) {
            fs << enemy.first.x() << "\n" << enemy.first.y() << "\n";
            fs << enemy.second.first << "\n" << enemy.second.second << "\n";
        }
        
        auto towers = game.getTowersInfo();
        fs << towers.size() << "\n";
        for (const auto& tower : towers) {
            fs << tower.first.x() << "\n" << tower.first.y() << "\n";
            fs << tower.second << "\n";
        }
        
        auto traps = game.getTrapsInfo();
        fs << traps.size() << "\n";
        for (const auto& trapPos : traps) {
            fs << trapPos.x() << "\n" << trapPos.y() << "\n";
        }
        
        fs << field.getWidth() << "\n" << field.getHeight() << "\n";
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                Position pos(x, y);
                fs << static_cast<int>(field.getCellType(pos)) << "\n";
            }
        }
        
        fs << "END_SAVE\n";
        
        std::cout << "Game saved: Level " << currentLevel 
                  << ", Score: " << player.getScore() 
                  << ", Health: " << player.getHealth() << "/" << player.getMaxHealth() << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Save error: " << e.what() << std::endl;
        return false;
    }
}

bool GameSaveSystem::loadGame(GameController& game, int& currentLevel, int& playerMaxHealth) {
    try {
        SaveFile file(filename, std::ios::in);
        auto& fs = file.get();
        
        std::string signature;
        std::getline(fs, signature);
        if (signature != "DUNGEON_CRAWLER_SAVE_V2") {
            throw LoadException("Invalid save file format");
        }
        
        fs >> currentLevel >> playerMaxHealth;
        
        int score, health, maxHealth, combatMode;
        int playerX, playerY;
        fs >> score >> health >> maxHealth >> combatMode;
        fs >> playerX >> playerY;
        
        if (fs.fail()) {
            throw CorruptedSaveException("Player data corrupted");
        }
        
        Player& player = game.getPlayer();
        player.addScore(score - player.getScore());
        player.setHealthPublic(health);
        player.setMaxHealthPublic(maxHealth);
        
        CombatMode savedMode = static_cast<CombatMode>(combatMode);
        if (savedMode != player.getCombatMode()) {
            player.switchCombatMode();
        }
        
        Position playerPos(playerX, playerY);
        GameField& field = game.getField();
        field.movePlayer(playerPos);
        player.setPosition(playerPos);
        
        int spellCount;
        fs >> spellCount;
        
        Hand& hand = player.getHand();
        while (hand.getSpellCount() > 0) {
            hand.removeSpell(0);
        }
        
        std::string spellName;
        std::getline(fs, spellName);
        
        for (int i = 0; i < spellCount; ++i) {
            std::getline(fs, spellName);
            std::unique_ptr<Spell> spell;
            
            if (spellName == "Heal") {
                spell.reset(new HealSpell());
            } else if (spellName == "Firebolt") {
                spell.reset(new DirectDamageSpell());
            } else if (spellName == "Fireball") {
                spell.reset(new AreaDamageSpell());
            } else if (spellName == "Trap") {
                spell.reset(new TrapSpell());
            }
            
            if (spell) {
                player.addSpell(std::move(spell));
            }
        }
        
        int enemyCount;
        fs >> enemyCount;
        game.clearEnemies();
        for (int i = 0; i < enemyCount; ++i) {
            int x, y, health, damage;
            fs >> x >> y >> health >> damage;
            if (fs.fail()) throw CorruptedSaveException("Enemy data corrupted");
            
            game.addEnemy(Position(x, y), health, damage);
        }
        
        int towerCount;
        fs >> towerCount;
        game.clearTowers();
        for (int i = 0; i < towerCount; ++i) {
            int x, y, cooldown;
            fs >> x >> y >> cooldown;
            if (fs.fail()) throw CorruptedSaveException("Tower data corrupted");
            game.addTower(Position(x, y), cooldown);
        }
        
        int trapCount;
        fs >> trapCount;
        auto currentTraps = field.getTraps();
        for (const auto& trap : currentTraps) {
            field.removeTrap(trap.first);
        }
        for (int i = 0; i < trapCount; ++i) {
            int x, y;
            fs >> x >> y;
            if (fs.fail()) throw CorruptedSaveException("Trap data corrupted");
            field.placeTrap(Position(x, y));
        }
        
        int fieldWidth, fieldHeight;
        fs >> fieldWidth >> fieldHeight;
        
        for (int y = 0; y < fieldHeight; ++y) {
            for (int x = 0; x < fieldWidth; ++x) {
                int cellType;
                fs >> cellType;
                if (fs.fail()) throw CorruptedSaveException("Field data corrupted");
                Position pos(x, y);
                field.updateCell(pos, static_cast<CellType>(cellType));
            }
        }
        
        std::string endMarker;
        std::getline(fs, endMarker);
        std::getline(fs, endMarker);
        if (endMarker != "END_SAVE") {
            throw CorruptedSaveException("Save file truncated");
        }
        
        std::cout << "Game loaded: Level " << currentLevel 
                  << ", Score: " << score 
                  << ", Health: " << health << "/" << maxHealth << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Load error: " << e.what() << std::endl;
        return false;
    }
}