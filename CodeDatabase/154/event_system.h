#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <iomanip>

enum class EventType {
    PlayerMoved,
    PlayerAttacked,
    PlayerTakeDamage,
    EnemySpawned,
    EnemyDefeated,
    BuildingBuilt,
    TrapTriggered,
    SpellLearned,
    SpellCasted,
    LevelCompleted,
    GameStarted,
    GameSaved,
    GameLoaded,
    GameEnded
};

struct GameEvent {
    EventType type;
    std::string description;
    std::chrono::system_clock::time_point timestamp;
    int entityId;
    int damage;
    int posRow;
    int posCol;

    GameEvent(EventType t, const std::string& desc, int id = -1, 
              int dmg = 0, int r = -1, int c = -1)
        : type(t), description(desc), timestamp(std::chrono::system_clock::now()),
          entityId(id), damage(dmg), posRow(r), posCol(c) {}

    std::string toString() const {
        auto time = std::chrono::system_clock::to_time_t(timestamp);
        std::stringstream ss;
        ss << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] "
           << description;
        return ss.str();
    }
};

class LoggerInterface {
public:
    virtual ~LoggerInterface() = default;
    virtual void log(const GameEvent& event) = 0;
};

class ConsoleLogger : public LoggerInterface {
public:
    void log(const GameEvent& event) override {
        std::cout << event.toString() << "\n";
    }
};

class FileLogger : public LoggerInterface {
private:
    std::string filename_;
    std::ofstream logFile_;

public:
    FileLogger(const std::string& filename) : filename_(filename) {
        logFile_.open(filename_, std::ios::app);
    }

    ~FileLogger() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

    void log(const GameEvent& event) override {
        if (logFile_.is_open()) {
            logFile_ << event.toString() << "\n";
            logFile_.flush();
        }
    }
};

class EventSystem {
private:
    std::vector<LoggerInterface*> loggers_;
    static EventSystem* instance_;

    EventSystem() = default;

public:
    static EventSystem& getInstance() {
        if (!instance_) {
            instance_ = new EventSystem();
        }
        return *instance_;
    }

    void addLogger(LoggerInterface* logger) {
        loggers_.push_back(logger);
    }

    void fireEvent(const GameEvent& event) {
        for (auto& logger : loggers_) {
            if (logger) logger->log(event);
        }
    }

    void logPlayerMoved(int row, int col) {
        GameEvent event(EventType::PlayerMoved, 
                       "Player moved to (" + std::to_string(row) + ", " + std::to_string(col) + ")",
                       0, 0, row, col);
        fireEvent(event);
    }

    void logPlayerAttacked(int targetId, int damage) {
        GameEvent event(EventType::PlayerAttacked,
                       "Player attacked entity " + std::to_string(targetId) + " for " + std::to_string(damage) + " damage",
                       targetId, damage);
        fireEvent(event);
    }

    void logPlayerTakeDamage(int damage, int attackerId) {
        GameEvent event(EventType::PlayerTakeDamage,
                       "Player took " + std::to_string(damage) + " damage from entity " + std::to_string(attackerId),
                       attackerId, damage);
        fireEvent(event);
    }

    void logEnemyDefeated(int enemyId) {
        GameEvent event(EventType::EnemyDefeated,
                       "Enemy " + std::to_string(enemyId) + " defeated",
                       enemyId);
        fireEvent(event);
    }

    void logEnemySpawned(int enemyId, int row, int col) {
        GameEvent event(EventType::EnemySpawned,
                       "Enemy " + std::to_string(enemyId) + " spawned at (" + std::to_string(row) + ", " + std::to_string(col) + ")",
                       enemyId, 0, row, col);
        fireEvent(event);
    }

    void logBuildingBuilt(int buildingId, int row, int col) {
        GameEvent event(EventType::BuildingBuilt,
                       "Building " + std::to_string(buildingId) + " built at (" + std::to_string(row) + ", " + std::to_string(col) + ")",
                       buildingId, 0, row, col);
        fireEvent(event);
    }

    void logTrapTriggered(int trapId, int targetId, int damage, int row, int col) {
        GameEvent event(EventType::TrapTriggered,
                       "Trap " + std::to_string(trapId) + " triggered against " + std::to_string(targetId) + " for " + std::to_string(damage) + " damage at (" + std::to_string(row) + ", " + std::to_string(col) + ")",
                       trapId, damage, row, col);
        fireEvent(event);
    }

    void logGameStarted() {
        GameEvent event(EventType::GameStarted, "Game started");
        fireEvent(event);
    }

    void logGameEnded() {
        GameEvent event(EventType::GameEnded, "Game ended");
        fireEvent(event);
    }

    void logSpellLearned(const std::string& spellName) {
        GameEvent event(EventType::SpellLearned,
                       "Player learned spell: " + spellName);
        fireEvent(event);
    }

    void logSpellCasted(const std::string& spellName) {
        GameEvent event(EventType::SpellCasted,
                       "Player casted spell: " + spellName);
        fireEvent(event);
    }

    void logLevelCompleted(int level) {
        GameEvent event(EventType::LevelCompleted,
                       "Level " + std::to_string(level) + " completed");
        fireEvent(event);
    }

    void logGameSaved(const std::string& filename) {
        GameEvent event(EventType::GameSaved,
                       "Game saved to " + filename);
        fireEvent(event);
    }

    void logGameLoaded(const std::string& filename) {
        GameEvent event(EventType::GameLoaded,
                       "Game loaded from " + filename);
        fireEvent(event);
    }

    void clear() {
        for (auto l : loggers_) {
            delete l;
        }
        loggers_.clear();
    }
    ~EventSystem() {
        clear();
    }
};

// Instance is defined in event_system.cpp to avoid multiple-definition when header is included by many TUs

#endif
