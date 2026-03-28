#ifndef INPUT_CONSOLEINPUTHANDLER_H
#define INPUT_CONSOLEINPUTHANDLER_H

#include "IInputHandler.h"
#include "../Commands/CommandType.h"
#include <map>
#include <string>

class ConsoleInputHandler : public IInputHandler {
    private:
        std::map<char, CommandType> keyMap;

        void loadControls(const std::string& filepath);

    public:
        explicit ConsoleInputHandler(const std::string& configPath = "");
        ~ConsoleInputHandler() override = default;

        std::unique_ptr<ICommand> getCommand() override;
};

#endif