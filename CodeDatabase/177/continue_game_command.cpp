#include "continue_game_command.h"
#include "game_controller.h"

bool ContinueGameCommand::execute(GameController& game) {
    (void)game; // Игнорируем параметр
    return true; // Продолжаем игру
}

std::string ContinueGameCommand::getDescription() const {
    return "Continue Game";
}

bool ContinueGameCommand::isValid() const {
    return true;
}

CommandType ContinueGameCommand::getCommandType() const {
    return CommandType::ContinueGame;
}

std::string ContinueGameCommand::getErrorMessage() const {
    return "";
}