#ifndef LOGGER_H
#define LOGGER_H

#include "GameObserver.h"
#include "GameEvent.h"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

enum class LogOutput {
    CONSOLE,
    FILE,
    BOTH
};

class GameLogger : public IGameObserver {
private:
    class LoggerImpl;
    std::unique_ptr<LoggerImpl> impl;
    
public:
    GameLogger(LogOutput output = LogOutput::BOTH, const std::string& filename = "game.log");
    ~GameLogger();
    
    void onEvent(const GameEvent& event) override;
    
    void onPlayerMoved(const Position& from, const Position& to) override;
    void onPlayerAttacked(const std::string& target, int damage) override;
    void onPlayerDamaged(int damage, const std::string& source) override;
    void onEnemyKilled(const Position& position, int coinsReward) override;
    void onTowerDestroyed(const Position& position) override;
    void onSpellUsed(const std::string& spellName, const Position& target) override;
    void onCommandExecuted(const std::string& command) override;
    void onLevelUp(int newLevel) override;
    void onGameSaved() override;
    void onGameLoaded() override;
    void onGameStarted() override;
    void onGameEnded() override;
    
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
    void logDebug(const std::string& message);
};

class Logger {
private:
    static std::unique_ptr<GameLogger> instance;
    static std::mutex instanceMutex;
    
    Logger() = delete;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
public:
    static void initialize(LogOutput output = LogOutput::BOTH, const std::string& filename = "game.log");
    static GameLogger& getInstance();
    static std::shared_ptr<GameLogger> getInstancePtr();
    static void destroy();
    static bool isInitialized();
};
#endif