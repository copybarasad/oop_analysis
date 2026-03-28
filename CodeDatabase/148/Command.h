#ifndef COMMAND_H
#define COMMAND_H

#include <string>

/**
 * @file Command.h
 * @brief Базовый интерфейс команды (паттерн Command)
 * 
 * Паттерн Command инкапсулирует запрос как объект, позволяя:
 * - Параметризовать клиентов различными запросами
 * - Ставить запросы в очередь
 * - Логировать запросы
 * - Поддерживать отмену операций (undo/redo)
 */

class Game; // Forward declaration

/**
 * @class Command
 * @brief Абстрактный класс команды
 */
class Command {
public:
    virtual ~Command() = default;
    
    /**
     * @brief Выполнить команду
     * @param game Указатель на игру
     * @return true если команда выполнена успешно
     */
    virtual bool execute(Game* game) = 0;
    
    /**
     * @brief Отменить команду (для undo)
     * @param game Указатель на игру
     * @return true если отмена выполнена успешно
     */
    virtual bool undo(Game* game) { 
        return false; // По умолчанию отмена не поддерживается
    }
    
    /**
     * @brief Можно ли отменить команду
     * @return true если команда поддерживает undo
     */
    virtual bool isUndoable() const { 
        return false; 
    }
    
    /**
     * @brief Получить описание команды для логирования
     * @return Строка с описанием
     */
    virtual std::string getDescription() const = 0;
};

#endif // COMMAND_H


