#include <limits>
#include <cctype>
#include <iostream>
#pragma once

#include "Command.h"
#include "ControlMapping.h"
#include "InputSystem.h"
#include <string>

class KeyboardCommandReader {
public:
    KeyboardCommandReader(const ControlMapping &mapping, InputSystem &input);

    Command readCommand();

    Direction readDirection();

    int readIndex();

    std::string directionPrompt() const;

private:
    const ControlMapping &controls;
    InputSystem &inputSystem;
};
