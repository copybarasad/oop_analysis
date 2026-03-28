#pragma once
#include <memory>

class Game;
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual bool Execute(Game& game) = 0;
};
