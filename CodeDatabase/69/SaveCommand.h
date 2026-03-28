#ifndef SAVE_COMMAND_H
#define SAVE_COMMAND_H

#include "Command.h"
#include <string>

class SaveCommand : public Command {
public:
    explicit SaveCommand(const std::string& filename);
    CommandResult execute(Game& game) override;
private:
    std::string filename_;
};

#endif
