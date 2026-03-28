#ifndef COMMANDINVOKER_H
#define COMMANDINVOKER_H

#include "Command.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @file CommandInvoker.h
 * @brief Исполнитель команд с поддержкой undo/redo
 */

class Game;

/**
 * @class CommandInvoker
 * @brief Управляет выполнением команд и историей
 * 
 * Responsibilities:
 * - Выполнение команд
 * - Хранение истории выполненных команд
 * - Поддержка undo/redo
 * - Ограничение размера истории
 */
class CommandInvoker {
private:
    std::vector<std::unique_ptr<Command>> history;  ///< История команд
    size_t currentPosition;  ///< Текущая позиция в истории (для undo/redo)
    size_t maxHistorySize;   ///< Максимальный размер истории
    
public:
    /**
     * @brief Конструктор
     * @param maxSize Максимальный размер истории (по умолчанию 50)
     */
    explicit CommandInvoker(size_t maxSize = 50);
    
    /**
     * @brief Выполнить команду
     * @param command Умный указатель на команду
     * @param game Указатель на игру
     * @return true если команда выполнена успешно
     * 
     * При успешном выполнении команда добавляется в историю.
     * Если команда не поддерживает undo, она не сохраняется.
     */
    bool executeCommand(std::unique_ptr<Command> command, Game* game);
    
    /**
     * @brief Отменить последнюю команду
     * @param game Указатель на игру
     * @return true если отмена выполнена успешно
     */
    bool undo(Game* game);
    
    /**
     * @brief Повторить отменённую команду
     * @param game Указатель на игру
     * @return true если повтор выполнен успешно
     */
    bool redo(Game* game);
    
    /**
     * @brief Можно ли отменить команду
     * @return true если есть команды для отмены
     */
    bool canUndo() const;
    
    /**
     * @brief Можно ли повторить команду
     * @return true если есть отменённые команды
     */
    bool canRedo() const;
    
    /**
     * @brief Очистить всю историю команд
     */
    void clearHistory();
    
    /**
     * @brief Получить историю команд в виде строк
     * @return Вектор описаний команд
     */
    std::vector<std::string> getHistory() const;
    
    /**
     * @brief Получить размер истории
     * @return Количество команд в истории
     */
    size_t getHistorySize() const { return currentPosition; }
};

#endif // COMMANDINVOKER_H


