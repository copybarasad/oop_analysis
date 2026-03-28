#ifndef SAVE_GAME_COMMAND_H
#define SAVE_GAME_COMMAND_H

#include "command.h"
#include <string>

class SaveGameCommand : public Command {
private:
    std::string filename;

public:
    explicit SaveGameCommand(const std::string& file) : filename(file) {}
    void execute(Game& game) override;
    const char* getDescription() const override { return "Save Game"; }
};

#endif