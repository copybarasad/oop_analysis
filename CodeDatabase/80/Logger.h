#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

enum class GameEventType {
    PLAYER_MOVE,
    PLAYER_ATTACK,
    PLAYER_USE_SPELL,
    PLAYER_DAMAGE_TAKEN,
    PLAYER_HEALED,
    PLAYER_LEVEL_UP,
    PLAYER_SPELL_GAINED,
    PLAYER_SPELL_USED,

    ENEMY_SPAWNED,
    ENEMY_MOVED,
    ENEMY_ATTACK,
    ENEMY_DAMAGE_TAKEN,
    ENEMY_DEFEATED,

    TOWER_SPAWN_ENEMY,
    TOWER_ATTACK,
    TOWER_DAMAGE_TAKEN,

    GAME_STARTED,
    GAME_SAVED,
    GAME_LOADED,
    LEVEL_STARTED,
    LEVEL_COMPLETED,
    GAME_OVER,
    VICTORY,

    SPELL_CAST,
    SPELL_DAMAGE,
    SPELL_AOE_DAMAGE,
    SPELL_TRAP_ACTIVATED
};

class IGameEventListener {
public:
    virtual ~IGameEventListener() = default;
    virtual void onGameEvent(GameEventType type, const std::string& description) = 0;
};

class Logger {
private:
    std::ofstream logFile;
    std::string logFilePath;
    LogLevel currentLevel;
    bool logToConsole;
    bool initialized;

    std::vector<IGameEventListener*> listeners;

    Logger() : currentLevel(LogLevel::INFO), logToConsole(false), initialized(false) {}
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getTimestamp();
    std::string levelToString(LogLevel level);
    std::string eventTypeToString(GameEventType type);

    void writeToFile(const std::string& message);
    void notifyListeners(GameEventType type, const std::string& description);

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    bool initialize(const std::string& filePath = "game.log",
        LogLevel level = LogLevel::INFO,
        bool consoleOutput = false);

    bool isInitialized() const { return initialized; }

    void log(LogLevel level, const std::string& message);

    void logEvent(GameEventType type, const std::string& description,
        const std::unordered_map<std::string, std::string>& details = {});

    void onGameEvent(GameEventType type, const std::string& description) {
        logEvent(type, description);
    }

    void addListener(IGameEventListener* listener);
    void removeListener(IGameEventListener* listener);

    static std::unordered_map<std::string, std::string> createDamageDetails(
        const std::string& attacker, const std::string& target, int damage);

    static std::unordered_map<std::string, std::string> createMoveDetails(
        int fromX, int fromY, int toX, int toY);

    static std::unordered_map<std::string, std::string> createSpellDetails(
        const std::string& spellName, int damage, int range);

    static std::unordered_map<std::string, std::string> createEntityDetails(
        const std::string& entityType, int x, int y, int health);
};

#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::DEBUG, msg)
#define LOG_INFO(msg) Logger::getInstance().log(LogLevel::INFO, msg)
#define LOG_WARNING(msg) Logger::getInstance().log(LogLevel::WARNING, msg)
#define LOG_ERROR(msg) Logger::getInstance().log(LogLevel::ERROR, msg)
#define LOG_CRITICAL(msg) Logger::getInstance().log(LogLevel::CRITICAL, msg)

#define LOG_EVENT(type, desc, details) Logger::getInstance().logEvent(type, desc, details)

#endif 