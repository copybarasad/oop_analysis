#ifndef GAME_LOGGER_H
#define GAME_LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "event_listener.h"

enum class LogOutput
{
    FILE
};

class GameLogger : public IEventListener
{
private:
    std::ofstream file_stream;
    
    std::string get_timestamp() const;
    std::string event_type_to_string(GameEventType type) const;
    void write_log(const std::string& message);

public:
    GameLogger(LogOutput output, const std::string& filepath);
    ~GameLogger();
    
    void on_event(const GameEvent& event) override;
};

#endif