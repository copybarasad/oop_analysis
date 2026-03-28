#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "command_interface.h"
#include "game_controller.h"
#include "string_utils.h"
#include "command.h"
#include "game_enums.h"
#include <istream>
#include <ostream>
#include <memory>
#include <optional>

class CommandFactory {
private:
    std::istream& inputStream;
    std::ostream& outputStream;
    StringUtils stringUtils;
    
    std::optional<int> readInt(const std::string& prompt, int minValue, int maxValue);
    std::optional<Position> readPosition(const std::string& prompt);
    std::string readLine();
    
    // Хардкод клавиш
    Direction getMovementDirection(char key) const;
    CommandType getInGameCommandType(char key) const;
    CommandType getMenuCommandType(char key) const;
    
public:
    CommandFactory(std::istream& input, std::ostream& output);
    
    // Основной метод для создания команд из ввода
    CommandPtr createCommandFromInput(const std::string& input, const GameController& game);
    
    // Методы для меню
    CommandPtr createMainMenuCommand(const std::string& input);
    CommandPtr createLoadMenuCommand(const std::string& input, int saveFilesCount);
    CommandPtr createInGameMenuCommand(const std::string& input);
};

#endif