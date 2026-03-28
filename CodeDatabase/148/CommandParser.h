#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <memory>
#include "Command.h"
#include "KeyBindingConfig.h"

/**
 * @file CommandParser.h
 * @brief Парсер пользовательского ввода в команды
 */

/**
 * @brief Статический класс для парсинга команд
 */
class CommandParser {
public:
    /**
     * @brief Парсить строку в команду с учетом конфигурации клавиш
     * @param input Строка пользовательского ввода
     * @param keyConfig Конфигурация привязки клавиш
     * @return Умный указатель на команду или nullptr
     */
    static std::unique_ptr<Command> parse(const std::string& input, 
                                          const KeyBindingConfig& keyConfig);
    
    /**
     * @brief Получить справку по командам (дефолт)
     * @return Строка с описанием всех команд
     */
    static std::string getHelp();
    
    /**
     * @brief Получить справку с учетом конфигурации клавиш
     * @param keyConfig Конфигурация клавиш
     * @return Строка с описанием всех команд
     */
    static std::string getHelp(const KeyBindingConfig& keyConfig);

private:
    /**
     * @brief Преобразовать строку в верхний регистр
     */
    static std::string toUpperCase(const std::string& str);
    
    /**
     * @brief Убрать пробелы в начале и конце строки
     */
    static std::string trim(const std::string& str);
};

#endif // COMMANDPARSER_H


