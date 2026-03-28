#pragma once
#include <memory>
#include "commands.h"

class InputHandler {
public:
    InputHandler(Command &command, bool &running)
        : command(command), running(running) {}

    void handleInput();

private:
    Command &command;
    bool &running;
};