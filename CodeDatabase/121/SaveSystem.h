#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "LevelController.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "GameField.h"
#include "Cell.h"
#include "SpellHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "GameExceptions.h"
#include "TextSystem.h"
#include "GameConfig.h"
#include "GameState.h"
#include <json.hpp>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

using json = nlohmann::json;

class SaveSystem {
private:
    std::shared_ptr<TextSystem> textSystem;
    std::string saveFilename;

public:
    // Конструктор с зависимостями
    SaveSystem(std::shared_ptr<TextSystem> textSystem, const std::string& filename = "savegame.json");
    
    // Основные методы
    void saveGame(const LevelController& level, int currentLevelNumber, GameState currentState, int turnCount);
    void loadGame(GameConfig& config, LevelController& level, int& currentLevelNumber, 
                  GameState& currentState, int& turnCount);
    
    // Геттер/сеттер для имени файла
    void setSaveFilename(const std::string& filename) { saveFilename = filename; }
    std::string getSaveFilename() const { return saveFilename; }

private:
    // Сериализация
    json serializeGameData(const LevelController& level, int currentLevelNumber, 
                          GameState currentState, int turnCount) const;
    json serializePlayer(const Player& player) const;
    json serializeGameField(const GameField& field) const;
    json serializeEnemies(const std::vector<Enemy>& enemies) const;
    json serializeBuildings(const std::vector<EnemyBuilding>& buildings) const;
    json serializeTowers(const std::vector<EnemyTower>& towers) const;
    json serializeSpellHand(const SpellHand& hand) const;
    json serializePosition(const Position& pos) const;
    
    // Десериализация
    void deserializeGameData(const json& j, GameConfig& config, LevelController& level,
                            int& currentLevelNumber, GameState& currentState, int& turnCount) const;
    void deserializePlayer(Player& player, const json& j) const;
    void deserializeGameField(GameField& field, const json& j) const;
    void deserializeEnemies(std::vector<Enemy>& enemies, const json& j, const GameConfig& config) const;
    void deserializeBuildings(std::vector<EnemyBuilding>& buildings, const json& j, const GameConfig& config) const;
    void deserializeTowers(std::vector<EnemyTower>& towers, const json& j, const GameConfig& config) const;
    void deserializeSpellHand(SpellHand& hand, const json& j) const;
    Position deserializePosition(const json& j) const;
    
    // Вспомогательные методы
    void validateSaveFile(const json& j) const;
    void backupExistingSave() const;
};

#endif
