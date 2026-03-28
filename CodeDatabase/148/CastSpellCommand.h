#ifndef CASTSPELLCOMMAND_H
#define CASTSPELLCOMMAND_H

#include "Command.h"

/**
 * @file CastSpellCommand.h
 * @brief Команда использования заклинания
 */

/**
 * @class CastSpellCommand
 * @brief Команда для использования заклинания из руки игрока
 * 
 * НЕ поддерживает undo (заклинания имеют необратимые эффекты).
 */
class CastSpellCommand : public Command {
private:
    int spellIndex;  ///< Индекс заклинания в руке (0-based)
    
public:
    /**
     * @brief Конструктор
     * @param index Индекс заклинания в руке игрока
     */
    explicit CastSpellCommand(int index) 
        : spellIndex(index) {}
    
    /**
     * @brief Выполнить использование заклинания
     */
    bool execute(Game* game) override;
    
    /**
     * @brief Заклинания не поддерживают undo
     */
    bool isUndoable() const override { 
        return false; 
    }
    
    /**
     * @brief Получить описание команды
     */
    std::string getDescription() const override;
};

#endif // CASTSPELLCOMMAND_H


