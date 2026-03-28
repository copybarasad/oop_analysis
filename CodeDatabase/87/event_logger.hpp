#ifndef EVENT_LOGGER_HPP
#define EVENT_LOGGER_HPP
#include "game_event.hpp"
#include <string>

class Event_logger{
    public:
        enum class Log_mode{NONE, FILE_ONLY, CONSOLE_ONLY};
        void initialize(Log_mode mode, const std::string& filename);
        void log_event(const Game_event& event);
        void shutdown();
    private:
        std::string log_filename;
        Log_mode mode = Log_mode::NONE;
};

#endif