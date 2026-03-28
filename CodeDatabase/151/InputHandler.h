#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Command.h"
#include <map>
#include <string>
#include <memory>

class ConsoleInputHandler {
private:
    std::map<char, std::string> keyToCommand;
    std::map<std::string, char> commandToKey;

    void loadDefaultConfig();
    bool loadConfigFromFile(const std::string& filename);

    Position getTargetFromUser() const;

public:
    ConsoleInputHandler(const std::string& configFile = "controls.config");

    std::unique_ptr<Command> getNextCommand();
};

#endif