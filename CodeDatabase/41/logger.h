#ifndef LOGGER_H
#define LOGGER_H

#include "event_system.h"
#include "enums.h"
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <memory>

class Logger : public Event_listener {
private:
    std::ofstream log_file;
    Log_output output_mode;
    std::mutex log_mutex;
    std::string log_file_name;
    
    static std::unique_ptr<Logger> logger_instance;
    
    void write_log(Log_level level, const std::string& message);
    std::string get_timestamp();
    std::string level_to_string(Log_level level);

public:
    Logger(const std::string& file_name = "game.log", Log_output mode = Log_output::BOTH);
    ~Logger();

    void on_event(const Game_event& event) override;
    
    void log(Log_level level, const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    
    void set_output_mode(Log_output mode);
    void change_log_file(const std::string& file_name);
    
    static void init(const std::string& file_name = "game.log", Log_output mode = Log_output::BOTH);
    static Logger& get_instance();
};

#endif