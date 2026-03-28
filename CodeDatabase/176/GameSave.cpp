#include "GameSave.h"
#include "GameController.h"
#include "GameManager.h"
#include "GameField.h"
#include "GameEnums.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>

GameSaveData::GameSaveData()
    : currentLevel(1), playerScore(0), playerHealth(100),
      playerMana(50), enemiesKilled(0), playerX(1), playerY(1),
      hasStartingSpell(false), playerCombatMode(0),
      fieldWidth(0), fieldHeight(0) {}

bool GameSaveData::isValid() const {
    if (currentLevel < 1 || currentLevel > 5) {
        return false;
    }
    if (playerHealth < 0 || playerHealth > 1000) {
        return false;
    }
    if (playerMana < 0 || playerMana > 1000) {
        return false;
    }
    if (enemiesKilled < 0) {
        return false;
    }
    
    if (fieldWidth < 3 || fieldWidth > 50 || fieldHeight < 3 || fieldHeight > 50) {
        return false;
    }
    
    if (playerX < 0 || playerX >= fieldWidth || playerY < 0 || playerY >= fieldHeight) {
        return false;
    }
    
    if (cellTypes.size() != static_cast<size_t>(fieldWidth * fieldHeight)) {
        return false;
    }
    
    for (int cellType : cellTypes) {
        if (cellType < 0 || cellType > 2) {
            return false;
        }
    }
    
    for (const auto& pos : enemyPositions) {
        if (pos.first < 0 || pos.first >= fieldWidth ||
            pos.second < 0 || pos.second >= fieldHeight) {
            return false;
        }
    }
    
    for (const auto& pos : trapPositions) {
        if (pos.first < 0 || pos.first >= fieldWidth ||
            pos.second < 0 || pos.second >= fieldHeight) {
            return false;
        }
    }
    
    for (const auto& pos : allyPositions) {
        if (pos.first < 0 || pos.first >= fieldWidth ||
            pos.second < 0 || pos.second >= fieldHeight) {
            return false;
        }
    }
    
    return true;
}

GameSave::GameSave(const std::string& fileName) : saveFileName(fileName) {}

bool GameSave::saveGame(const GameController& controller) {
    try {
        GameSaveData data;
        
        const auto& gameManager = controller.getGameManager();
        const auto& field = gameManager.getField();
        
        data.currentLevel = controller.getCurrentLevel();
        data.playerScore = controller.getPlayerScore();
        data.playerHealth = gameManager.getPlayer().getHealth();
        data.playerMana = gameManager.getPlayer().getMana();
        data.enemiesKilled = gameManager.getPlayer().getEnemiesKilled();
        
        auto playerPos = gameManager.getPlayerPosition();
        data.playerX = playerPos.first;
        data.playerY = playerPos.second;
        
        auto* spellHand = gameManager.getPlayer().getSpellHand();
        data.hasStartingSpell = (spellHand && spellHand->getSpellCount() > 0);
        data.playerCombatMode = (gameManager.getPlayer().getCombatMode() == CombatMode::MELEE) ? 0 : 1;
        
        data.fieldWidth = field.getWidth();
        data.fieldHeight = field.getHeight();
        
        data.cellTypes.clear();
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                const auto& cell = field.getCell(x, y);
                switch (cell.getType()) {
                    case CellType::EMPTY: data.cellTypes.push_back(0); break;
                    case CellType::OBSTACLE: data.cellTypes.push_back(1); break;
                    case CellType::SLOW: data.cellTypes.push_back(2); break;
                }
            }
        }
        
        data.enemyPositions.clear();
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                if (field.hasEnemyAt(x, y)) {
                    data.enemyPositions.emplace_back(x, y);
                }
            }
        }
        
        data.trapPositions = field.getTrapPositions();
        data.allyPositions = field.getAllyPositions();
        
        validateSaveData(data);
        
        saveToFile(data);
        
        std::cout << "Game saved successfully!\n";
        return true;
        
    } catch (const SaveException& e) {
        std::cout << "Save failed: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error during save: " << e.what() << "\n";
        return false;
    }
}

void GameSave::saveToFile(const GameSaveData& data) const {
    std::ofstream file(saveFileName, std::ios::binary);
    if (!file.is_open()) {
        throw FileOpenException(saveFileName);
    }
    
    try {
        file.write(reinterpret_cast<const char*>(&data.currentLevel), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.playerScore), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.playerHealth), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.playerMana), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.enemiesKilled), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.playerX), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.playerY), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.hasStartingSpell), sizeof(bool));
        file.write(reinterpret_cast<const char*>(&data.playerCombatMode), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.fieldWidth), sizeof(int));
        file.write(reinterpret_cast<const char*>(&data.fieldHeight), sizeof(int));

        int cellCount = data.cellTypes.size();
        file.write(reinterpret_cast<const char*>(&cellCount), sizeof(int));
        for (int type : data.cellTypes) {
            file.write(reinterpret_cast<const char*>(&type), sizeof(int));
        }
        
        int enemyCount = data.enemyPositions.size();
        file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(int));
        for (const auto& pos : data.enemyPositions) {
            file.write(reinterpret_cast<const char*>(&pos.first), sizeof(int));
            file.write(reinterpret_cast<const char*>(&pos.second), sizeof(int));
        }
        
        int trapCount = data.trapPositions.size();
        file.write(reinterpret_cast<const char*>(&trapCount), sizeof(int));
        for (const auto& pos : data.trapPositions) {
            file.write(reinterpret_cast<const char*>(&pos.first), sizeof(int));
            file.write(reinterpret_cast<const char*>(&pos.second), sizeof(int));
        }
        
        int allyCount = data.allyPositions.size();
        file.write(reinterpret_cast<const char*>(&allyCount), sizeof(int));
        for (const auto& pos : data.allyPositions) {
            file.write(reinterpret_cast<const char*>(&pos.first), sizeof(int));
            file.write(reinterpret_cast<const char*>(&pos.second), sizeof(int));
        }
        
        if (!file.good()) {
            throw FileWriteException(saveFileName);
        }
        
    } catch (const std::exception&) {
        file.close();
        throw;
    }
}

std::unique_ptr<GameSaveData> GameSave::loadGame() {
    try {
        auto data = loadFromFile();
        if (data) {
            validateSaveData(*data);
            std::cout << "Game loaded successfully!\n";
        }
        return data;
        
    } catch (const SaveNotFoundException& e) {
        std::cout << "Load failed: " << e.what() << "\n";
        return nullptr;
    } catch (const SaveException& e) {
        std::cout << "Load failed: " << e.what() << "\n";
        return nullptr;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error during load: " << e.what() << "\n";
        return nullptr;
    }
}

std::unique_ptr<GameSaveData> GameSave::loadFromFile() const {
    std::ifstream file(saveFileName, std::ios::binary);
    if (!file.is_open()) {
        throw SaveNotFoundException(saveFileName);
    }
    
    auto data = std::make_unique<GameSaveData>();
    
    try {
        file.read(reinterpret_cast<char*>(&data->currentLevel), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->playerScore), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->playerHealth), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->playerMana), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->enemiesKilled), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->playerX), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->playerY), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->hasStartingSpell), sizeof(bool));
        file.read(reinterpret_cast<char*>(&data->playerCombatMode), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->fieldWidth), sizeof(int));
        file.read(reinterpret_cast<char*>(&data->fieldHeight), sizeof(int));
        
        if (!file.good()) {
            throw FileCorruptedException(saveFileName);
        }

        int cellCount;
        file.read(reinterpret_cast<char*>(&cellCount), sizeof(int));
        if (cellCount < 0 || cellCount > 10000) { // Разумный лимит
            throw FileCorruptedException(saveFileName);
        }
        
        data->cellTypes.resize(cellCount);
        for (int i = 0; i < cellCount; ++i) {
            file.read(reinterpret_cast<char*>(&data->cellTypes[i]), sizeof(int));
        }

        int enemyCount;
        file.read(reinterpret_cast<char*>(&enemyCount), sizeof(int));
        if (enemyCount < 0 || enemyCount > 1000) {
            throw FileCorruptedException(saveFileName);
        }
        
        data->enemyPositions.resize(enemyCount);
        for (int i = 0; i < enemyCount; ++i) {
            file.read(reinterpret_cast<char*>(&data->enemyPositions[i].first), sizeof(int));
            file.read(reinterpret_cast<char*>(&data->enemyPositions[i].second), sizeof(int));
        }

        int trapCount;
        file.read(reinterpret_cast<char*>(&trapCount), sizeof(int));
        if (trapCount < 0 || trapCount > 1000) {
            throw FileCorruptedException(saveFileName);
        }
        
        data->trapPositions.resize(trapCount);
        for (int i = 0; i < trapCount; ++i) {
            file.read(reinterpret_cast<char*>(&data->trapPositions[i].first), sizeof(int));
            file.read(reinterpret_cast<char*>(&data->trapPositions[i].second), sizeof(int));
        }
        
        int allyCount;
        file.read(reinterpret_cast<char*>(&allyCount), sizeof(int));
        if (allyCount < 0 || allyCount > 1000) {
            throw FileCorruptedException(saveFileName);
        }
        
        data->allyPositions.resize(allyCount);
        for (int i = 0; i < allyCount; ++i) {
            file.read(reinterpret_cast<char*>(&data->allyPositions[i].first), sizeof(int));
            file.read(reinterpret_cast<char*>(&data->allyPositions[i].second), sizeof(int));
        }
        
        if (!file.good()) {
            throw FileCorruptedException(saveFileName);
        }
        
    } catch (const std::exception&) {
        file.close();
        throw;
    }
    
    return data;
}

void GameSave::validateSaveData(const GameSaveData& data) const {
    if (!data.isValid()) {
        throw DataValidationException("Save data failed validation checks");
    }
}

bool GameSave::saveExists() const {
    try {
        std::ifstream file(saveFileName);
        return file.good();
    } catch (const std::exception&) {
        return false;
    }
}
