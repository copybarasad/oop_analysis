#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

class GameLogger 
{
private:
    static std::ofstream log_file;
    static bool enabled;
    
public:
    static void initialize();
    static void logEvent(const std::string& event);
    static void logError(const std::string& error);
    static void close();
    
private:
    static std::string getCurrentTime();
};