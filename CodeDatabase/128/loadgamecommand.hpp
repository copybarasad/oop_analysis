#pragma once
#include "icommand.hpp"
#include <string>

class LoadGameCommand : public ICommand {
public:
    explicit LoadGameCommand(std::string filename);
    bool Execute(Game& game) override;

private:
    std::string filename_;
};