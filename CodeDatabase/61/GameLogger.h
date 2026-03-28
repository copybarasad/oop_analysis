#ifndef GAME_LOGGER_H
#define GAME_LOGGER_H

#include "EventManager.h"
#include "GameEvent.h"
#include <string>
#include <fstream>
#include <memory>
#include <ostream>

// Режимы логирования
enum class LogMode {
    CONSOLE,    // Логирование в консоль
    FILE,       // Логирование в файл
    BOTH        // Логирование и в консоль, и в файл
};

// Система логирования игровых событий
class GameLogger : public IEventSubscriber {
private:
    LogMode mode;
    std::string logFileName;
    std::ofstream logFile;
    bool isFileOpen;
    
    void logToConsole(const std::string& message);
    void logToFile(const std::string& message);
    
public:
    GameLogger(LogMode mode = LogMode::CONSOLE, const std::string& filename = "game.log");
    ~GameLogger();
    
    // IEventSubscriber interface
    void onEvent(const std::shared_ptr<GameEvent>& event) override;
    
    // Управление логированием
    void setMode(LogMode newMode);
    void setLogFile(const std::string& filename);
    void closeLogFile();
    
    // Статический метод для создания логгера из параметров командной строки
    static std::unique_ptr<GameLogger> createFromArgs(int argc, char* argv[]);
};

#endif // GAME_LOGGER_H

