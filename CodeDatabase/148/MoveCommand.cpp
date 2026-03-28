#include "MoveCommand.h"
#include "Game.h"
#include "Player.h"

/**
 * @file MoveCommand.cpp
 * @brief Реализация команды движения
 */

/**
 * @brief Выполнить команду движения
 * @param game Указатель на игру
 * @return true если движение выполнено успешно
 * 
 * Процесс:
 * 1. Сохраняем текущую позицию игрока (для undo)
 * 2. Выполняем движение через Game::processPlayerTurn()
 * 3. Отмечаем успех для возможности undo
 */
bool MoveCommand::execute(Game* game) {
    if (!game || !game->getPlayer()) {
        return false;
    }
    
    Player* player = game->getPlayer();
    
    // Сохраняем текущую позицию для undo
    auto [currentX, currentY] = player->getPosition();
    previousX = currentX;
    previousY = currentY;
    
    // Вычисляем новую позицию
    int newX = currentX;
    int newY = currentY;
    
    switch (direction) {
        case Direction::UP:    newY--; break;
        case Direction::DOWN:  newY++; break;
        case Direction::LEFT:  newX--; break;
        case Direction::RIGHT: newX++; break;
    }
    
    // Проверяем валидность хода
    if (!game->isValidMove(newX, newY)) {
        moveSucceeded = false;
        return false;
    }
    
    // Выполняем ход через игровую систему
    game->processPlayerTurn(direction);
    
    // Проверяем изменилась ли позиция
    auto [afterX, afterY] = player->getPosition();
    moveSucceeded = (afterX != previousX || afterY != previousY);
    
    return moveSucceeded;
}

/**
 * @brief Отменить движение
 * @param game Указатель на игру
 * @return true если отмена выполнена успешно
 * 
 * Телепортирует игрока на предыдущую сохранённую позицию.
 * ВНИМАНИЕ: Не отменяет побочные эффекты (бой, триггеры ловушек и т.д.)
 */
bool MoveCommand::undo(Game* game) {
    if (!moveSucceeded || !game || !game->getPlayer()) {
        return false;
    }
    
    Player* player = game->getPlayer();
    
    // Возвращаем игрока на предыдущую позицию
    player->setPosition(previousX, previousY);
    
    return true;
}

/**
 * @brief Получить строковое описание команды
 * @return Строка вида "Move UP" или "Move DOWN"
 */
std::string MoveCommand::getDescription() const {
    std::string dirStr;
    
    switch (direction) {
        case Direction::UP:    dirStr = "UP"; break;
        case Direction::DOWN:  dirStr = "DOWN"; break;
        case Direction::LEFT:  dirStr = "LEFT"; break;
        case Direction::RIGHT: dirStr = "RIGHT"; break;
        default:               dirStr = "UNKNOWN";
    }
    
    return "Move " + dirStr;
}


