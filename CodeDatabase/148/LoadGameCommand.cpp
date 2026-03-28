#include "LoadGameCommand.h"
#include "Game.h"
#include "Exceptions.h"

/**
 * @file LoadGameCommand.cpp
 * @brief Реализация команды загрузки игры
 */

/**
 * @brief Выполнить загрузку игры
 * @param game Указатель на игру
 * @return true если загрузка прошла успешно
 * 
 * Использует Game::loadGame(), который загружает состояние
 * из файла saves/<name>.save.
 */
bool LoadGameCommand::execute(Game* game) {
    if (!game) {
        return false;
    }
    
    try {
        game->loadGame(saveName);
        return true;
    } catch (const GameException& e) {
        // Ошибка загрузки (обрабатывается в UI)
        return false;
    }
}

/**
 * @brief Получить описание команды
 * @return Строка вида "Load Game: quicksave"
 */
std::string LoadGameCommand::getDescription() const {
    return "Load Game: " + saveName;
}


