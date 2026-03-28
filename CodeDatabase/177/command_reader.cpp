#include "command_reader.h"
#include "game_controller.h"
#include "back_to_main_menu_command.h"  
#include "quit_game_command.h"
#include "invalid_command.h"
#include <sstream>

CommandReader::CommandReader(std::istream& input, std::ostream& output)
    : in(input), out(output), commandFactory(input, output) {}

bool CommandReader::readLine(std::string& line) {
    return static_cast<bool>(std::getline(in, line));
}

//Чтение команд из главного меню
CommandPtr CommandReader::readMainMenuCommand() {
    std::string line;
    if (!readLine(line)) {
        return std::make_unique<QuitGameCommand>();
    }
    return commandFactory.createMainMenuCommand(line);
}

//Чтение выбора из меню загрузки
CommandPtr CommandReader::readLoadMenuCommand(int saveFilesCount) {
    std::string line;
    if (!readLine(line)) {
        return std::make_unique<BackToMainMenuCommand>();
    }
    return commandFactory.createLoadMenuCommand(line, saveFilesCount);
}

//Чтение игровых команд
CommandPtr CommandReader::readInGameCommand(const GameController& game) {
    std::string line;
    if (!readLine(line)) {
        return std::make_unique<QuitGameCommand>();
    }
    return commandFactory.createCommandFromInput(line, game);
}

//Чтение команд из меню паузы
CommandPtr CommandReader::readInGameMenuCommand() {
    std::string line;
    if (!readLine(line)) {
        return std::make_unique<InvalidCommand>("Failed to read command");
    }
    return commandFactory.createInGameMenuCommand(line);
}