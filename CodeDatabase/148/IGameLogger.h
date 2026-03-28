#ifndef IGAMELOGGER_H
#define IGAMELOGGER_H

#include "GameEvent.h"

/**
 * @file IGameLogger.h
 * @brief Интерфейс логгера игровых событий
 */

/**
 * @interface IGameLogger
 * @brief Абстрактный интерфейс для различных логгеров
 * 
 * Observer в паттерне Observer.
 * Позволяет реализовать:
 * - Логирование в файл
 * - Логирование в консоль
 * - Логирование по сети
 * - Множественное логирование
 */
class IGameLogger {
public:
    virtual ~IGameLogger() = default;
    
    /**
     * @brief Записать событие в лог
     * @param event Игровое событие
     */
    virtual void logEvent(const GameEvent& event) = 0;
    
    /**
     * @brief Закрыть логгер (flush данных)
     */
    virtual void close() {}
};

#endif // IGAMELOGGER_H


