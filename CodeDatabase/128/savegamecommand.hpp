#pragma once
#include "icommand.hpp"
#include <string>

class SaveGameCommand : public ICommand {
public:
    explicit SaveGameCommand(std::string filename);
    bool Execute(Game& game) override;

private:
    std::string filename_;
};