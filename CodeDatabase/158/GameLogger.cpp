#include "GameLogger.h"

std::ofstream GameLogger::log_file;
bool GameLogger::enabled = false;

void GameLogger::initialize() 
{
    log_file.open("game_log.txt", std::ios::app);
    if (log_file.is_open()) 
    {
        log_file << "\n=== NEW GAME SESSION " << getCurrentTime() << " ===\n";
        enabled = true;
    } 
    else 
    {
        enabled = false;
    }
}

void GameLogger::logEvent(const std::string& event) 
{
    if (enabled && log_file.is_open()) 
    {
        log_file << "[" << getCurrentTime() << "] " << event << "\n";
        log_file.flush();
    }
}

void GameLogger::logError(const std::string& error) 
{
    if (enabled && log_file.is_open()) 
    {
        log_file << "[" << getCurrentTime() << "] ERROR: " << error << "\n";
        log_file.flush();
        std::cerr << "ERROR: " << error << "\n";
    }
}

void GameLogger::close() 
{
    if (log_file.is_open()) 
    {
        log_file << "=== END SESSION ===\n\n";
        log_file.close();
    }
}

std::string GameLogger::getCurrentTime() 
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}