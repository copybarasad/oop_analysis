#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "Command.h"
#include "GameTypes.h"

/**
 * @file MoveCommand.h
 * @brief Команда движения игрока
 */

/**
 * @class MoveCommand
 * @brief Команда для перемещения игрока в указанном направлении
 * 
 * Поддерживает отмену (undo) - возврат на предыдущую позицию.
 */
class MoveCommand : public Command {
private:
    Direction direction;  ///< Направление движения
    
    // Данные для undo
    int previousX;  ///< Предыдущая X-координата
    int previousY;  ///< Предыдущая Y-координата
    bool moveSucceeded;  ///< Удалось ли выполнить движение
    
public:
    /**
     * @brief Конструктор
     * @param dir Направление движения (UP/DOWN/LEFT/RIGHT)
     */
    explicit MoveCommand(Direction dir) 
        : direction(dir), 
          previousX(-1), 
          previousY(-1), 
          moveSucceeded(false) {}
    
    /**
     * @brief Выполнить движение
     */
    bool execute(Game* game) override;
    
    /**
     * @brief Отменить движение (вернуться на предыдущую позицию)
     */
    bool undo(Game* game) override;
    
    /**
     * @brief Команда поддерживает undo только если движение было успешным
     */
    bool isUndoable() const override { 
        return moveSucceeded; 
    }
    
    /**
     * @brief Получить описание команды
     */
    std::string getDescription() const override;
};

#endif // MOVECOMMAND_H


