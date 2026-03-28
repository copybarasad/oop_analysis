#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include "event_listener.h"
#include <fstream>
#include <memory>
#include <string>

class EventLogger : public EventListener {
public:
    virtual ~EventLogger() = default;
    virtual void onEvent(const std::shared_ptr<GameEvent>& event) override = 0;
};

class ConsoleEventLogger : public EventLogger {
public:
    void onEvent(const std::shared_ptr<GameEvent>& event) override;
};

class FileEventLogger : public EventLogger {
private:
    std::string filename;
    mutable std::ofstream logFile;
    
    void ensureFileOpen() const;

public:
    explicit FileEventLogger(const std::string& logFilename);
    ~FileEventLogger();
    
    void onEvent(const std::shared_ptr<GameEvent>& event) override;
};

#endif