#pragma once
#include <string>
#include <memory>
#include "GameState.h"

class SaveLoadException : public std::exception {
private:
    std::string message_;
public:
    explicit SaveLoadException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};

class FileNotFoundException : public SaveLoadException {
public:
    FileNotFoundException() : SaveLoadException("Save file not found") {}
};

class CorruptedSaveException : public SaveLoadException {
public:
    CorruptedSaveException() : SaveLoadException("Save file is corrupted") {}
};

class WritePermissionException : public SaveLoadException {
public:
    WritePermissionException() : SaveLoadException("No write permission for save file") {}
};

class SaveLoadManager {
public:
    static std::unique_ptr<GameState> loadGame(const std::string& filename);
    static void saveGame(const GameState& state, const std::string& filename);
    
private:
    static void validateSaveData(const std::string& data);
    static std::string serializeGameState(const GameState& state);
    static std::unique_ptr<GameState> deserializeGameState(const std::string& data);
    
    static std::string serializeCell(int x, int y, const Cell& cell);
    static void deserializeCell(const std::string& data, Field& field);
    
    static std::string serializeEnemy(std::shared_ptr<Enemy> enemy);
    static std::shared_ptr<Enemy> deserializeEnemy(const std::string& data);
    
    static std::string serializeEnemyBase(std::shared_ptr<EnemyBase> base);
    static std::shared_ptr<EnemyBase> deserializeEnemyBase(const std::string& data);
    
    static std::string serializeEnemyTower(std::shared_ptr<EnemyTower> tower);
    static std::shared_ptr<EnemyTower> deserializeEnemyTower(const std::string& data);
    
    static std::string serializeAlly(std::shared_ptr<Ally> ally);
    static std::shared_ptr<Ally> deserializeAlly(const std::string& data);
};