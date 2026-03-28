#ifndef LOAD_COMMAND_H
#define LOAD_COMMAND_H

#include "Command.h"
#include <string>

class LoadCommand : public Command {
public:
    explicit LoadCommand(const std::string& filename);
    CommandResult execute(Game& game) override;
private:
    std::string filename_;
};

#endif
