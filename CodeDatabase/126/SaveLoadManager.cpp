#include "../../headers/managers/SaveLoadManager.h"
#include "../../headers/objects/Player.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/objects/Trap.h"
#include "../../headers/spells/DirectDamageSpell.h"
#include "../../headers/spells/AreaDamageSpell.h"
#include "../../headers/spells/TrapSpell.h"
#include "../../headers/exceptions/GameException.h"
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;

SaveLoadManager::SaveLoadManager(GameState& state) : gameState(state) {}

void SaveLoadManager::saveGame(const string& filename) {
    ofstream file(filename);
    if(!file.is_open()){
        throw SaveGameException(filename, "Cannot open file for writing");    
    }

    try {
        file << gameState.getCurrentLevel() << "\n";
        file << gameState.getField().getWidth() << " " << gameState.getField().getHeight() << "\n";
        file << gameState.getPlayer()->serialize() << "\n";
        file << gameState.getPlayer()->getSpellHand().serialize() << "\n";

        auto& gameObjects = gameState.getGameObjects();
        file << gameObjects.size() << "\n";
        for (const auto& obj : gameObjects) {
            file << obj->serialize() << "\n";
        }

        file << static_cast<int>(gameState.getStatus()) << "\n";
        
        file << gameState.getField().serialize() << "\n";
    }
    catch (const exception& e) {
        throw SaveGameException(filename, string("Write error: ") + e.what());
    }

    cout << "Game saved successfully to " << filename << "\n";
}

void SaveLoadManager::loadGame(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }
    
    try {
        int savedLevel;
        file >> savedLevel;
        if (file.fail()) throw CorruptedSaveException(filename, "Invalid level");
        
        int savedWidth, savedHeight;
        file >> savedWidth >> savedHeight;
        if (file.fail()) throw CorruptedSaveException(filename, "Invalid field dimensions");
        
        gameState.setField(GameField(savedWidth, savedHeight));
        gameState.setCurrentLevel(savedLevel);
        
        string playerData;
        getline(file, playerData);
        getline(file, playerData);
        gameState.setPlayer(Player::deserialize(playerData));
        
        gameState.getPlayer()->getSpellHand().deserialize(file);
        
        gameState.clearGameObjects();
        int objectCount;
        file >> objectCount;
        if (file.fail()) throw CorruptedSaveException(filename, "Invalid object count");
        
        string line;
        getline(file, line);
        
        map<string, function<shared_ptr<GameObject>(const string&)>> objectFactories = {
            {"Enemy", Enemy::deserialize},
            {"Tower", EnemyTower::deserialize},
            {"Trap", Trap::deserialize}
        };
        
        for (int i = 0; i < objectCount; i++) {
            if (!getline(file, line)) {
                throw CorruptedSaveException(filename, "Unexpected end of file");
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string type;
            ss >> type;
            
            if (objectFactories.find(type) != objectFactories.end()) {
                try {
                    gameState.addGameObject(objectFactories[type](line));
                } catch (const exception& e) {
                    cout << "Warning: Failed to load object: " << e.what() << endl;
                }
            }
        }
        
        int statusValue;
        if (file >> statusValue) {
            gameState.setStatus(static_cast<GameStatus>(statusValue));
        } else {
            gameState.setStatus(GameStatus::RUNNING);
        }
        
        getline(file, line);
        gameState.getField().deserialize(file);
        
        gameState.setGameRunning(true);
        
    } catch (const exception& e) {
        throw CorruptedSaveException(filename, e.what());
    }
    
    gameState.updateFieldState();
    cout << "Game loaded successfully from " << filename << "\n";
}