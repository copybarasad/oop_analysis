#pragma once
#include "Command.h"
#include <string>
#include <utility>

class SaveCommand : public Command {
public:
    explicit SaveCommand(std::string filename) : filename_(std::move(filename)) {}
    void execute(GameCycle& game) override {
        game.saveGame(filename_);
    }
private:
    std::string filename_;
};
