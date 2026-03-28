#include "../include/GameEventLogger.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

GameEventLogger::GameEventLogger() : isEnabled(false), target(LogTarget::NONE) {}

GameEventLogger::~GameEventLogger() {
    disable();
}

GameEventLogger& GameEventLogger::getInstance() {
    static GameEventLogger instance;
    return instance;
}

void GameEventLogger::enable(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    logFilePath = filename;
    logFile.open(filename, std::ios::out | std::ios::app);
    
    if (logFile.is_open()) {
        isEnabled = true;
        target = LogTarget::FILE;
        logFile << "\n=== Game Session Started at " << getTimestamp() << " ===\n";
        logFile.flush();
    }
}

void GameEventLogger::enableConsole() {
    std::lock_guard<std::mutex> lock(logMutex);
    isEnabled = true;
    target = LogTarget::CONSOLE;
    std::cout << "\n=== Game Logging to Console ===\n";
}

void GameEventLogger::enableBoth(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    logFilePath = filename;
    logFile.open(filename, std::ios::out | std::ios::app);
    
    if (logFile.is_open()) {
        isEnabled = true;
        target = LogTarget::BOTH;
        logFile << "\n=== Game Session Started at " << getTimestamp() << " ===\n";
        logFile.flush();
        std::cout << "\n=== Game Logging to File and Console ===\n";
    }
}

void GameEventLogger::setTarget(LogTarget newTarget, const std::string& filename) {
    switch (newTarget) {
        case LogTarget::FILE:
            enable(filename);
            break;
        case LogTarget::CONSOLE:
            enableConsole();
            break;
        case LogTarget::BOTH:
            enableBoth(filename);
            break;
        case LogTarget::NONE:
            disable();
            break;
    }
}

void GameEventLogger::disable() {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (isEnabled && logFile.is_open()) {
        logFile << "=== Game Session Ended at " << getTimestamp() << " ===\n\n";
        logFile.close();
    }
    isEnabled = false;
    target = LogTarget::NONE;
}

std::string GameEventLogger::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string GameEventLogger::eventTypeToString(GameEventType type) const {
    switch (type) {
        case GameEventType::COMMAND_INPUT: return "COMMAND_INPUT";
        case GameEventType::PLAYER_MOVED: return "PLAYER_MOVED";
        case GameEventType::PLAYER_ATTACKED: return "PLAYER_ATTACKED";
        case GameEventType::PLAYER_DAMAGED: return "PLAYER_DAMAGED";
        case GameEventType::PLAYER_MODE_SWITCHED: return "PLAYER_MODE_SWITCHED";
        case GameEventType::PLAYER_SPELL_CAST: return "PLAYER_SPELL_CAST";
        case GameEventType::PLAYER_SPELL_BOUGHT: return "PLAYER_SPELL_BOUGHT";
        case GameEventType::PLAYER_UPGRADE_CHOSEN: return "PLAYER_UPGRADE_CHOSEN";
        
        case GameEventType::ENEMY_MOVED: return "ENEMY_MOVED";
        case GameEventType::ENEMY_ATTACKED: return "ENEMY_ATTACKED";
        case GameEventType::ENEMY_DAMAGED: return "ENEMY_DAMAGED";
        case GameEventType::ENEMY_KILLED: return "ENEMY_KILLED";
        case GameEventType::ENEMY_SPAWNED: return "ENEMY_SPAWNED";
        
        case GameEventType::BUILDING_DAMAGED: return "BUILDING_DAMAGED";
        case GameEventType::BUILDING_DESTROYED: return "BUILDING_DESTROYED";
        case GameEventType::BUILDING_SPAWNED_ENEMY: return "BUILDING_SPAWNED_ENEMY";
        
        case GameEventType::TOWER_ATTACKED: return "TOWER_ATTACKED";
        case GameEventType::TOWER_DAMAGED: return "TOWER_DAMAGED";
        case GameEventType::TOWER_DESTROYED: return "TOWER_DESTROYED";
        
        case GameEventType::ALLY_MOVED: return "ALLY_MOVED";
        case GameEventType::ALLY_ATTACKED: return "ALLY_ATTACKED";
        case GameEventType::ALLY_DAMAGED: return "ALLY_DAMAGED";
        case GameEventType::ALLY_KILLED: return "ALLY_KILLED";
        case GameEventType::ALLY_SUMMONED: return "ALLY_SUMMONED";
        
        case GameEventType::TRAP_PLACED: return "TRAP_PLACED";
        case GameEventType::TRAP_TRIGGERED: return "TRAP_TRIGGERED";
        
        case GameEventType::LEVEL_STARTED: return "LEVEL_STARTED";
        case GameEventType::LEVEL_COMPLETED: return "LEVEL_COMPLETED";
        case GameEventType::GAME_OVER: return "GAME_OVER";
        case GameEventType::GAME_VICTORY: return "GAME_VICTORY";
        case GameEventType::GAME_SAVED: return "GAME_SAVED";
        case GameEventType::GAME_LOADED: return "GAME_LOADED";
        
        case GameEventType::TURN_STARTED: return "TURN_STARTED";
        case GameEventType::TURN_ENDED: return "TURN_ENDED";
        
        default: return "UNKNOWN";
    }
}

std::string GameEventLogger::formatEvent(const GameEvent& event) const {
    std::ostringstream oss;
    
    oss << "[" << getTimestamp() << "] ";
    oss << eventTypeToString(event.type);
    
    if (event.position.getX() != 0 || event.position.getY() != 0) {
        oss << " at (" << event.position.getX() << "," << event.position.getY() << ")";
    }
    
    if (event.targetPosition.getX() != 0 || event.targetPosition.getY() != 0) {
        oss << " -> (" << event.targetPosition.getX() << "," << event.targetPosition.getY() << ")";
    }
    
    if (event.value1 != 0) {
        oss << " [value=" << event.value1 << "]";
    }
    
    if (!event.description.empty()) {
        oss << " : " << event.description;
    }
    
    return oss.str();
}

void GameEventLogger::onEvent(const GameEvent& event) {
    if (!isEnabled) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex);
    std::string formatted = formatEvent(event);
    
    if (target == LogTarget::FILE || target == LogTarget::BOTH) {
        if (logFile.is_open()) {
            logFile << formatted << "\n";
            logFile.flush();
        }
    }
    
    if (target == LogTarget::CONSOLE || target == LogTarget::BOTH) {
        std::cout << formatted << "\n";
    }
}

void GameEventLogger::logCustomMessage(const std::string& message) {
    if (!isEnabled) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex);
    std::string formatted = "[" + getTimestamp() + "] CUSTOM: " + message;
    
    if (target == LogTarget::FILE || target == LogTarget::BOTH) {
        if (logFile.is_open()) {
            logFile << formatted << "\n";
        }
    }
    
    if (target == LogTarget::CONSOLE || target == LogTarget::BOTH) {
        std::cout << formatted << "\n";
    }
}

void GameEventLogger::flush() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile.flush();
    }
}
