#include "event_logger.hpp"
#include <iostream>
#include <fstream>

void Event_logger::initialize(Log_mode m, const std::string& filename){
    mode = m;
    log_filename = filename;
    if(mode == Log_mode::FILE_ONLY){
        std::ofstream file(log_filename, std::ios::trunc);
        file.close();
    }
}

void Event_logger::log_event(const Game_event& event) {
    if(mode == Log_mode::NONE) return;
    std::string message = event.to_string();
    if(message.empty()) return;
    if(mode == Log_mode::CONSOLE_ONLY) std::cout << "LOG " << message << "\n";
    else if(mode == Log_mode::FILE_ONLY){
        std::ofstream file(log_filename, std::ios::app);
        file << message << "\n";
    }
}

void Event_logger::shutdown(){
    mode = Log_mode::NONE;
}