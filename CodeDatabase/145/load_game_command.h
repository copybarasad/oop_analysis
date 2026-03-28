#ifndef LOAD_GAME_COMMAND_H
#define LOAD_GAME_COMMAND_H

#include "command.h"
#include <string>

class LoadGameCommand : public Command {
private:
    std::string filename;

public:
    explicit LoadGameCommand(const std::string& file) : filename(file) {}
    void execute(Game& game) override;
    const char* getDescription() const override { return "Load Game"; }
};

#endif