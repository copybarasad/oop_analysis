#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <memory>

// Переименовали константы, чтобы не конфликтовать с windows.h
enum class LogLevel {
    L_INFO,
    L_WARNING,
    L_ERROR,
    L_GAME_EVENT
};

class Logger {
private:
    std::ofstream logFile;
    bool logToFile;
    bool logToConsole;
    
    std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buffer);
    }
    
    std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::L_INFO: return "INFO";
            case LogLevel::L_WARNING: return "WARNING";
            case LogLevel::L_ERROR: return "ERROR";
            case LogLevel::L_GAME_EVENT: return "GAME_EVENT";
            default: return "UNKNOWN";
        }
    }
    
public:
    Logger(bool toFile = true, bool toConsole = true, const std::string& filename = "game_log.txt")
        : logToFile(toFile), logToConsole(toConsole) {
        
        if (logToFile) {
            logFile.open(filename, std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "Не удалось открыть файл лога: " << filename << std::endl;
                logToFile = false;
            } else {
                log("=== НАЧАЛО СЕАНСА ===", LogLevel::L_INFO);
            }
        }
    }
    
    ~Logger() {
        if (logToFile && logFile.is_open()) {
            log("=== КОНЕЦ СЕАНСА ===", LogLevel::L_INFO);
            logFile.close();
        }
    }
    
    // Основной метод лога
    void log(const std::string& message, LogLevel level = LogLevel::L_INFO) {
        std::stringstream logEntry;
        logEntry << "[" << getCurrentTime() << "] "
                 << "[" << levelToString(level) << "] "
                 << message;
        
        if (logToConsole) {
            switch (level) {
                case LogLevel::L_INFO:
                    std::cout << "\033[36m" << logEntry.str() << "\033[0m" << std::endl;
                    break;
                case LogLevel::L_WARNING:
                    std::cout << "\033[33m" << logEntry.str() << "\033[0m" << std::endl;
                    break;
                case LogLevel::L_ERROR:
                    std::cout << "\033[31m" << logEntry.str() << "\033[0m" << std::endl;
                    break;
                case LogLevel::L_GAME_EVENT:
                    std::cout << "\033[32m" << logEntry.str() << "\033[0m" << std::endl;
                    break;
                default:
                    std::cout << logEntry.str() << std::endl;
            }
        }
        
        if (logToFile && logFile.is_open()) {
            logFile << logEntry.str() << std::endl;
            logFile.flush();
        }
    }
    
    // --- СПЕЦИАЛЬНЫЕ МЕТОДЫ (Добавлены, чтобы Game.cpp не ругался) ---
    
    void logPlayerMove(const std::string& playerName, int fromX, int fromY, int toX, int toY) {
        std::string msg = playerName + " переместился с (" + std::to_string(fromX) + "," + std::to_string(fromY) + 
                          ") на (" + std::to_string(toX) + "," + std::to_string(toY) + ")";
        log(msg, LogLevel::L_GAME_EVENT);
    }
    
    void logAttack(const std::string& attacker, const std::string& target, int damage) {
        std::string msg = attacker + " атаковал " + target + " на " + std::to_string(damage) + " урона";
        log(msg, LogLevel::L_GAME_EVENT);
    }
    
    void logSpellCast(const std::string& caster, const std::string& spellName, int x, int y) {
        std::string msg = caster + " применил заклинание " + spellName + " на (" + 
                          std::to_string(x) + "," + std::to_string(y) + ")";
        log(msg, LogLevel::L_GAME_EVENT);
    }
};

#endif // LOGGER_H