#ifndef SAVEGAMECOMMAND_H
#define SAVEGAMECOMMAND_H

#include "Command.h"
#include <string>

/**
 * @file SaveGameCommand.h
 * @brief Команда сохранения игры
 */

/**
 * @class SaveGameCommand
 * @brief Команда для сохранения состояния игры в файл
 * 
 * НЕ поддерживает undo (сохранение файла необратимо).
 */
class SaveGameCommand : public Command {
private:
    std::string saveName;  ///< Имя сохранения
    
public:
    /**
     * @brief Конструктор
     * @param name Имя сохранения (без расширения)
     */
    explicit SaveGameCommand(const std::string& name) 
        : saveName(name) {}
    
    /**
     * @brief Выполнить сохранение игры
     */
    bool execute(Game* game) override;
    
    /**
     * @brief Сохранение не поддерживает undo
     */
    bool isUndoable() const override { 
        return false; 
    }
    
    /**
     * @brief Получить описание команды
     */
    std::string getDescription() const override;
};

#endif // SAVEGAMECOMMAND_H


