#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>

class Logger {
    std::ofstream file;
    
    public:
    Logger();
    
    void log(const std::string& message);
    
    ~Logger();
};

#endif