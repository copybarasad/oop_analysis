#ifndef INVALID_COMMAND_H
#define INVALID_COMMAND_H

#include "command_interface.h"
#include <string>

class InvalidCommand : public ICommand {
private:
    std::string errorMessage;
    
public:
    explicit InvalidCommand(const std::string& error = "Invalid command");
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    std::string getErrorMessage() const override;
    CommandType getCommandType() const override { return CommandType::Invalid; }
};

#endif