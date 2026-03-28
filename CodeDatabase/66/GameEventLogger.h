#ifndef GAMEEVENTLOGGER_H
#define GAMEEVENTLOGGER_H

#include "GameEvent.h"
#include <fstream>
#include <string>
#include <chrono>
#include <mutex>
#include <iostream>

enum class LogTarget {
    NONE,
    FILE,
    CONSOLE,
    BOTH
};

class GameEventLogger : public IGameEventListener {
private:
    std::ofstream logFile;
    bool isEnabled;
    std::string logFilePath;
    std::mutex logMutex;
    LogTarget target;
    
    std::string getTimestamp() const;
    std::string eventTypeToString(GameEventType type) const;
    std::string formatEvent(const GameEvent& event) const;
    
    GameEventLogger();
    ~GameEventLogger();
    
public:
    static GameEventLogger& getInstance();
    
    GameEventLogger(const GameEventLogger&) = delete;
    GameEventLogger& operator=(const GameEventLogger&) = delete;
    
    void enable(const std::string& filename = "game_log.txt");
    void enableConsole();
    void enableBoth(const std::string& filename = "game_log.txt");
    void setTarget(LogTarget newTarget, const std::string& filename = "game_log.txt");
    void disable();
    bool isLogging() const { return isEnabled; }
    LogTarget getTarget() const { return target; }
    
    void onEvent(const GameEvent& event) override;
    
    void logCustomMessage(const std::string& message);
    void flush();
};

#endif
