#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <string>
#include <unordered_map>

/**
 * @file CommandLineParser.h
 * @brief Парсер аргументов командной строки
 */

/**
 * @enum LogMode
 * @brief Режим логирования
 */
enum class LogMode {
    NONE,       ///< Без логирования
    CONSOLE,    ///< Логирование в консоль
    FILE,       ///< Логирование в файл
    BOTH        ///< Логирование в консоль и файл
};

/**
 * @struct CommandLineArgs
 * @brief Распарсенные аргументы командной строки
 */
struct CommandLineArgs {
    LogMode logMode = LogMode::CONSOLE;           ///< Режим логирования
    std::string logFile = "logs/game.log";        ///< Путь к файлу лога
    std::string configFile = "keybindings.txt";  ///< Путь к конфигу клавиш
    bool showHelp = false;                        ///< Показать справку
};

/**
 * @class CommandLineParser
 * @brief Парсер аргументов запуска программы
 * 
 * Поддерживаемые аргументы:
 * - --log-to=console|file|both|none
 * - --log-file=путь/к/файлу.log
 * - --config=путь/к/конфигу.txt
 * - --help или -h
 */
class CommandLineParser {
public:
    /**
     * @brief Распарсить аргументы командной строки
     * @param argc Количество аргументов
     * @param argv Массив аргументов
     * @return Структура с распарсенными аргументами
     */
    static CommandLineArgs parse(int argc, char* argv[]);
    
    /**
     * @brief Показать справку по использованию
     */
    static void printHelp();
    
private:
    /**
     * @brief Разбить аргумент формата --key=value
     * @param arg Аргумент
     * @return Пара (key, value)
     */
    static std::pair<std::string, std::string> splitArgument(const std::string& arg);
};

#endif // COMMANDLINEPARSER_H


