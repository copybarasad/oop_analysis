#include "CommandInvoker.h"
#include "Game.h"

/**
 * @file CommandInvoker.cpp
 * @brief Реализация исполнителя команд
 */

/**
 * @brief Конструктор
 * @param maxSize Максимальный размер истории
 */
CommandInvoker::CommandInvoker(size_t maxSize) 
    : currentPosition(0), maxHistorySize(maxSize) {
    history.reserve(maxSize);
}

/**
 * @brief Выполнить команду
 * @param command Умный указатель на команду
 * @param game Указатель на игру
 * @return true если команда выполнена успешно
 * 
 * Процесс:
 * 1. Выполняем команду
 * 2. Если успешно и команда поддерживает undo - добавляем в историю
 * 3. Удаляем отменённые команды после текущей позиции
 * 4. Ограничиваем размер истории
 */
bool CommandInvoker::executeCommand(std::unique_ptr<Command> command, Game* game) {
    if (!command || !game) {
        return false;
    }
    
    // Выполняем команду
    bool success = command->execute(game);
    
    if (success && command->isUndoable()) {
        // Удаляем все команды после текущей позиции (если делали undo)
        if (currentPosition < history.size()) {
            history.erase(history.begin() + currentPosition, history.end());
        }
        
        // Добавляем команду в историю
        history.push_back(std::move(command));
        currentPosition++;
        
        // Ограничиваем размер истории
        if (history.size() > maxHistorySize) {
            history.erase(history.begin());
            currentPosition--;
        }
    }
    
    return success;
}

/**
 * @brief Отменить последнюю команду
 * @param game Указатель на игру
 * @return true если отмена выполнена успешно
 */
bool CommandInvoker::undo(Game* game) {
    if (!canUndo() || !game) {
        return false;
    }
    
    // Получаем последнюю выполненную команду
    Command* command = history[currentPosition - 1].get();
    
    // Отменяем
    bool success = command->undo(game);
    
    if (success) {
        currentPosition--;
    }
    
    return success;
}

/**
 * @brief Повторить отменённую команду
 * @param game Указатель на игру
 * @return true если повтор выполнен успешно
 */
bool CommandInvoker::redo(Game* game) {
    if (!canRedo() || !game) {
        return false;
    }
    
    // Получаем следующую команду после текущей позиции
    Command* command = history[currentPosition].get();
    
    // Выполняем снова
    bool success = command->execute(game);
    
    if (success) {
        currentPosition++;
    }
    
    return success;
}

/**
 * @brief Можно ли отменить
 * @return true если есть выполненные команды
 */
bool CommandInvoker::canUndo() const {
    return currentPosition > 0;
}

/**
 * @brief Можно ли повторить
 * @return true если есть отменённые команды
 */
bool CommandInvoker::canRedo() const {
    return currentPosition < history.size();
}

/**
 * @brief Очистить историю команд
 */
void CommandInvoker::clearHistory() {
    history.clear();
    currentPosition = 0;
}

/**
 * @brief Получить историю в виде строк
 * @return Вектор описаний команд
 */
std::vector<std::string> CommandInvoker::getHistory() const {
    std::vector<std::string> result;
    result.reserve(currentPosition);
    
    for (size_t i = 0; i < currentPosition; ++i) {
        result.push_back(history[i]->getDescription());
    }
    
    return result;
}


