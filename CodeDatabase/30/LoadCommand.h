#pragma once
#include "Command.h"
#include <string>
#include <utility>

class LoadCommand : public Command {
public:
    explicit LoadCommand(std::string filename) : filename_(std::move(filename)) {}
    void execute(GameCycle& game) override {
        game.loadGame(filename_);
    }
private:
    std::string filename_;
};
