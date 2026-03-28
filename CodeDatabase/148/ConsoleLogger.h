#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "IGameLogger.h"
#include <iostream>

/**
 * @file ConsoleLogger.h
 * @brief Логгер в консоль
 */

/**
 * @class ConsoleLogger
 * @brief Выводит события в стандартный вывод
 */
class ConsoleLogger : public IGameLogger {
private:
    bool showTimestamp;  ///< Показывать ли временные метки
    bool showType;       ///< Показывать ли тип события
    
public:
    /**
     * @brief Конструктор
     * @param timestamp Показывать временные метки
     * @param type Показывать тип события
     */
    explicit ConsoleLogger(bool timestamp = true, bool type = true)
        : showTimestamp(timestamp), showType(type) {}
    
    /**
     * @brief Записать событие в консоль
     */
    void logEvent(const GameEvent& event) override {
        std::cout << "[LOG] ";
        
        if (showTimestamp) {
            std::cout << "[" << event.getTimestampString() << "] ";
        }
        
        if (showType) {
            std::cout << "[" << event.getTypeString() << "] ";
        }
        
        std::cout << event.description << "\n";
    }
};

#endif // CONSOLELOGGER_H


