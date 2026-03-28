#ifndef LOADGAMECOMMAND_H
#define LOADGAMECOMMAND_H

#include "Command.h"
#include <string>

/**
 * @file LoadGameCommand.h
 * @brief Команда загрузки игры
 */

/**
 * @class LoadGameCommand
 * @brief Команда для загрузки состояния игры из файла
 * 
 * НЕ поддерживает undo (загрузка полностью перезаписывает состояние).
 */
class LoadGameCommand : public Command {
private:
    std::string saveName;  ///< Имя сохранения
    
public:
    /**
     * @brief Конструктор
     * @param name Имя сохранения (без расширения)
     */
    explicit LoadGameCommand(const std::string& name) 
        : saveName(name) {}
    
    /**
     * @brief Выполнить загрузку игры
     */
    bool execute(Game* game) override;
    
    /**
     * @brief Загрузка не поддерживает undo
     */
    bool isUndoable() const override { 
        return false; 
    }
    
    /**
     * @brief Получить описание команды
     */
    std::string getDescription() const override;
};

#endif // LOADGAMECOMMAND_H


