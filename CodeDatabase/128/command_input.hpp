#pragma once
#include <memory>
#include <iostream>
#include <string>
#include "icommand.hpp"
#include "key_config.hpp"

class QuitCommand;
class MoveCommand;
class ToggleModeCommand;
class SaveGameCommand;
class LoadGameCommand;
class UseSpellCommand;

class CommandInput {
public:
    CommandInput();
    std::unique_ptr<ICommand> ReadCommand();

private:
    KeyConfig config_;
};
