#include "Logger.h"
#include <algorithm>
#include <ctime>

class GameLogger::LoggerImpl {
private:
    LogOutput outputType;
    std::ofstream logFile;
    std::string filename;
    LogLevel minLogLevel;
    std::mutex logMutex;
    
    std::string getCurrentTime() const {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        std::tm tm;
        #ifdef _WIN32
        localtime_s(&tm, &time);
        #else
        localtime_r(&time, &tm);
        #endif
        
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
    std::string getLogLevelString(LogLevel level) const {
        switch(level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::DEBUG: return "DEBUG";
            default: return "UNKNOWN";
        }
    }
    
    LogLevel getLogLevelForEvent(EventType type) const {
        switch(type) {
            case EventType::PLAYER_DAMAGED:
            case EventType::GAME_ENDED:
                return LogLevel::WARNING;
            case EventType::COMMAND_EXECUTED:
            case EventType::PLAYER_MOVED:
                return LogLevel::DEBUG;
            default:
                return LogLevel::INFO;
        }
    }
    
public:
    LoggerImpl(LogOutput output, const std::string& file = "game.log", LogLevel minLevel = LogLevel::INFO)
        : outputType(output), filename(file), minLogLevel(minLevel) {
        if (outputType == LogOutput::FILE || outputType == LogOutput::BOTH) {
            logFile.open(filename, std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "Warning: Could not open log file " << filename << std::endl;
                outputType = LogOutput::CONSOLE;
            }
        }
    }
    
    ~LoggerImpl() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void log(const GameEvent& event) {
        std::lock_guard<std::mutex> lock(logMutex);
        
        LogLevel level = getLogLevelForEvent(event.type);
        if (level < minLogLevel) {
            return;
        }
        
        std::string timestamp = getCurrentTime();
        std::string levelStr = getLogLevelString(level);
        std::string message = event.toString();
        
        std::string logEntry = timestamp + " [" + levelStr + "] " + message;
        
        if (outputType == LogOutput::CONSOLE || outputType == LogOutput::BOTH) {
            std::cout << logEntry << std::endl;
        }
        
        if ((outputType == LogOutput::FILE || outputType == LogOutput::BOTH) && logFile.is_open()) {
            logFile << logEntry << std::endl;
            logFile.flush();
        }
    }
    
    void logDirect(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        
        if (level < minLogLevel) {
            return;
        }
        
        std::string timestamp = getCurrentTime();
        std::string levelStr = getLogLevelString(level);
        
        std::string logEntry = timestamp + " [" + levelStr + "] " + message;
        
        if (outputType == LogOutput::CONSOLE || outputType == LogOutput::BOTH) {
            std::cout << logEntry << std::endl;
        }
        
        if ((outputType == LogOutput::FILE || outputType == LogOutput::BOTH) && logFile.is_open()) {
            logFile << logEntry << std::endl;
            logFile.flush();
        }
    }
};

GameLogger::GameLogger(LogOutput output, const std::string& filename) {
    impl = std::make_unique<LoggerImpl>(output, filename);
}

GameLogger::~GameLogger() = default;

void GameLogger::onEvent(const GameEvent& event) {
    impl->log(event);
}

void GameLogger::onPlayerMoved(const Position& from, const Position& to) {
    impl->log(PlayerMovedEvent(from, to));
}

void GameLogger::onPlayerAttacked(const std::string& target, int damage) {
    impl->log(PlayerAttackedEvent(target, damage));
}

void GameLogger::onPlayerDamaged(int damage, const std::string& source) {
    impl->log(PlayerDamagedEvent(damage, source));
}

void GameLogger::onEnemyKilled(const Position& position, int coinsReward) {
    impl->log(EnemyKilledEvent(position, coinsReward));
}

void GameLogger::onTowerDestroyed(const Position& position) {
    impl->log(TowerDestroyedEvent(position));
}

void GameLogger::onSpellUsed(const std::string& spellName, const Position& target) {
    impl->log(SpellUsedEvent(spellName, target));
}

void GameLogger::onCommandExecuted(const std::string& command) {
    impl->log(CommandExecutedEvent(command));
}

void GameLogger::onLevelUp(int newLevel) {
    impl->log(GameEvent(EventType::LEVEL_UP, 
                       "Player leveled up to level " + std::to_string(newLevel)));
}

void GameLogger::onGameSaved() {
    impl->log(GameEvent(EventType::GAME_SAVED, "Game saved"));
}

void GameLogger::onGameLoaded() {
    impl->log(GameEvent(EventType::GAME_LOADED, "Game loaded"));
}

void GameLogger::onGameStarted() {
    impl->log(GameEvent(EventType::GAME_STARTED, "Game started"));
}

void GameLogger::onGameEnded() {
    impl->log(GameEvent(EventType::GAME_ENDED, "Game ended"));
}

void GameLogger::logInfo(const std::string& message) {
    impl->logDirect(LogLevel::INFO, message);
}

void GameLogger::logWarning(const std::string& message) {
    impl->logDirect(LogLevel::WARNING, message);
}

void GameLogger::logError(const std::string& message) {
    impl->logDirect(LogLevel::ERROR, message);
}

void GameLogger::logDebug(const std::string& message) {
    impl->logDirect(LogLevel::DEBUG, message);
}

std::unique_ptr<GameLogger> Logger::instance = nullptr;
std::mutex Logger::instanceMutex;

void Logger::initialize(LogOutput output, const std::string& filename) {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (!instance) {
        instance = std::make_unique<GameLogger>(output, filename);
    }
}

GameLogger& Logger::getInstance() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (!instance) {
        initialize();
    }
    return *instance;
}

std::shared_ptr<GameLogger> Logger::getInstancePtr() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (!instance) {
        initialize();
    }
    return std::shared_ptr<GameLogger>(std::shared_ptr<GameLogger>{}, &(*instance));
}

void Logger::destroy() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    instance.reset();
}

bool Logger::isInitialized() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    return instance != nullptr;
}