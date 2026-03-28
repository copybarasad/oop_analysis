#pragma once
#include "InputProcessor.h"
#include "MoveCommand.h"
#include "SwitchCombatModeCommand.h"
#include "CastSpellCommand.h"
#include "PauseCommand.h"

class ConsoleInputProcessor : public InputProcessor {
public:
    std::unique_ptr<Command> processInput() override;

private:
    char getRawInput();
    std::unique_ptr<Command> convertToCommand(char input);
};