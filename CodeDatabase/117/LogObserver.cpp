#include "LogObserver.h"
#include <iostream>
#include <stdexcept>

LogObserver::LogObserver(LogType type, const std::string& filename): type(type) {
    if (type == LogType::InFile){
        try{
            this->logFile = std::make_unique<LogFileWrapper>(filename);
        }
        catch(std::runtime_error& e){
            std::cout << "Logs will be written to the console because " << e.what() << std::endl;
        this->type = LogType::Console;
        }
    }
}

void LogObserver::seeEvent(const Event& event) {
    std::string message = event.toString();
    
    if (this->type == LogType::InFile) {
        try{
            this->logFile->write(message);
        }
        catch(std::runtime_error& e){
            std::cout << e.what();
        }
    }
    else{
        std::cout << message << std::endl;
    }
}