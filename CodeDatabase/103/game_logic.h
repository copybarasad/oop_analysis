#pragma once

#include "general_manager.h"
#include "save_system.h"
#include "input_source.h"
#include "renderer_interface.h"
#include "direct_damage_spell.h"
#include "console_input_source.cpp"
#include "console_render.cpp"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <cstdlib>
#include <string>

template<typename InputSource, typename Renderer>
class GameLogic {
private:
    GeneralManager manager;
    int level;
    SaveSystem saveSystem;
    
    InputSource inputSource;
    Renderer renderer;
    
public:
    GameLogic(int width, int height, int enemies, int lvl = 1) 
        : manager(width, height, enemies), level(lvl) {
    }
    
    void run() {
        if (!startMenu()) {
            return;
        }

        while (!(isGameOver())) {
            renderer.clearScreen();
            renderer.printMessage("=== LEVEL " + std::to_string(level) + " ===");

            renderer.render(manager);
            renderer.printStatus(manager);
            
            if (isLevelComplete()) {
                renderer.printMessage("Level " + std::to_string(level) + " completed!");
                nextLevel();
                levelUpMenu();
                continue;
            }
            
            renderer.printMessage("Enter direction (w=up, s=down, a=left, d=right, c=change fight type,\n p=pause, f=use spells, g=get spell)");
            Direction dir = inputSource.getDirection();
            
            if (dir == Direction::CHANGE) {
                manager.switchPlayerFightType();
                continue;
            }
            
            if (dir == Direction::SPELL) {
                if (useSpells()) continue;
            }
            
            if (dir == Direction::PAUSE) {
                if (processingPause()) continue;
                break;
            }
            
            if (dir == Direction::GET_SPELL) {
                getRandSpell();
                continue;
            }
            
            if (dir == Direction::LOAD || dir == Direction::SAVE || dir == Direction::QUIT) {
                continue; 
            }
            
            manager.playerTurn(dir);
            manager.enemiesTurn();
        }
    }
    
private:
    void nextLevel() {
        level++;
        
        PlayerManager& playerMan = manager.getPlayerManager();
        playerMan.healPlayer(playerMan.getPlayerMaxHealth());
        
        auto& spellHand = playerMan.getSpellHandRef();
        auto spells = spellHand.getSpells();
        int spellsToRemove = spells.size() / 2;
        
        for (int i = 0; i < spellsToRemove && !spells.empty(); i++) {
            int randomIndex = rand() % spells.size();
            spellHand.removeSpell(randomIndex);
        }
        
        int newWidth = 10 + level;
        int newHeight = 10 + level;
        int newEnemies = 2 + level;
        int newTowers = 1 + level;
        
        manager = GeneralManager(newWidth, newHeight, newEnemies, newTowers, &playerMan);
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    void levelUpMenu() {
        renderer.clearScreen();
        renderer.printLvlUpMenu();

        PlayerManager& playerMan = manager.getPlayerManager();
        
        char choice = inputSource.getCharImmediate();
        
        switch(choice) {
            case '1':
                playerMan.setPlayerMaxHealth(playerMan.getPlayerMaxHealth() + 20);
                playerMan.healPlayer(playerMan.getPlayerMaxHealth());
                renderer.printMessage("\nHealth increased to " + std::to_string(playerMan.getPlayerMaxHealth()));
                break;
            case '2':
                playerMan.setPlayerDamage(playerMan.getPlayerDamage() + 5);
                renderer.printMessage("\nDamage increased to " + std::to_string(playerMan.getPlayerDamage()));
                break;
            case '3':
                playerMan.upgradePlayerRandomSpell();
                break;
            case '4':
                renderer.printMessage("\nUpgrade skipped");
                break;
            default:
                renderer.printMessage("\nInvalid choice, upgrade skipped");
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    bool isLevelComplete() {
        EnemyManager& enemyMan = manager.getEnemyManager();
        TowerManager& towerMan = manager.getTowerManager();
        
        return enemyMan.getAliveEnemyPositions().size() == 0 &&
               towerMan.getAllTowers().size() == 0;
    }
    
    bool startMenu() {
        renderer.clearScreen();
        renderer.printStartMenu();
        
        char choice = inputSource.getChar();
        
        switch(choice) {
            case '1': {
                renderer.clearScreen();
                manager = GeneralManager(10, 10, 2, 1);
                level = 1;
                return true;
            }
            case '2': {
                renderer.clearScreen();
                return attemptToLoad();
            }
            case '3':
                renderer.printMessage("Goodbye!");
                return false;
            default:
                renderer.printMessage("Invalid choice. Starting new game...");
                std::this_thread::sleep_for(std::chrono::seconds(2));
                level = 1;
                manager = GeneralManager(10, 10, 2, 1);
                return true;
        }
    }
    
    bool useSpells() {
        const auto& spells = manager.getSpells();
        if (spells.empty()){
            renderer.printMessage("None spells, press enter to continue");
            std::cin.ignore();
            std::cin.get();
            return false;
        } 

        renderer.printMessage("\nAvailable Spells:");
        for (size_t i = 0; i < spells.size(); ++i) {
            renderer.printMessage( std::to_string(i) +   ": " + spells[i]->getName()
                      + " (cost: " + std::to_string(spells[i]->getCost())
                      + " , damage: " + std::to_string(spells[i]->getDamage())
                      + " , radius: " + std::to_string(spells[i]->getRadius()) + ")");
        }

        int spellIndex;
        renderer.printMessage("Choose spell index (-1 to skip): ");
        spellIndex = inputSource.getInt();

        if (spellIndex >= 0 && spellIndex < spells.size()) {
            int x, y;
            renderer.printMessage("Enter target coordinates (x y): ");
            x = inputSource.getInt();;
            y = inputSource.getInt();;

            manager.castSpell(spellIndex, {x, y});
            return true;
        }
        return false;
    }
    
    bool isGameOver() {
        PlayerManager& playerMan = manager.getPlayerManager();
        EnemyManager& enemyMan = manager.getEnemyManager();
        TowerManager& towerMan = manager.getTowerManager();
        
        auto player = playerMan.getPlayer();
        if (player && !player->isAlive()) { 
            renderer.printMessage("Player died!");
            renderer.printMessage("Game Over!");
            
            renderer.printMessage("Restart? (y/n): ");
            char restart = inputSource.getChar();
            if (restart == 'y' || restart == 'Y') {
                if (startMenu()) return false; 
                return true;
            }
            return true;
        }

        return false; 
    }
    
    void getRandSpell() {
        PlayerManager& playerMan = manager.getPlayerManager();
        auto player = playerMan.getPlayer();
        if (!player) return;

        if (player->getPoints() >= 10) {
            int spellType = rand() % 3;
            ISpell* newSpell = nullptr;
            
            switch(spellType) {
                case 0: 
                    newSpell = new DirectDamageSpell(50, 10);
                    break;
                case 1:
                    newSpell = new AreaDamageSpell(50, 10);
                    break;
                case 2:
                    newSpell = new TrapSpell(50, 10);
                    break;
            }
            
            playerMan.addPlayerSpell(newSpell);
            
            renderer.printMessage("You have received a spell. Press enter to continue...");
            std::cin.ignore();
            std::cin.get();
        } else {
            renderer.printMessage("=== Need 10 points for a random spell! ===");
            renderer.printMessage("You have: " + std::to_string(player->getPoints()) + " points. Press enter to continue...");
            std::cin.ignore();
            std::cin.get();
        }
    }
    
    bool processingPause() {
        renderer.clearScreen();
        renderer.printMessage("You in pause, press v - to save, l - to load and q - to quit");
        std::string filename;
        Direction dir = inputSource.getDirection();
        if (dir == Direction::SAVE) {
            renderer.printMessage("Write a filename to save(.dat):");
            filename = inputSource.getString();
            saveSystem.setName(filename);
            saveSystem.saveGame(manager, level);
            renderer.printMessage("Game saved!");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            return true;
        }
        if (dir == Direction::LOAD){
            return attemptToLoad();
        }
        return false;
    }
    
    bool attemptToLoad() {
        renderer.printMessage("Your saves:");
        for (const auto& entry : std::filesystem::directory_iterator("../saves")) {
            if (entry.path().extension() == ".dat") {
                std::cout << entry.path().filename().string() << std::endl;
            }
        }
        std::string filename;
        renderer.printMessage("Write a filename to load:");
        filename = inputSource.getString();
        saveSystem.setName(filename);
        if (saveSystem.loadGame(manager, level)) {
            renderer.printMessage("Game loaded! Level: " + std::to_string(level));
        } else {
            renderer.printMessage("Load failed!");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return true;
    }
};