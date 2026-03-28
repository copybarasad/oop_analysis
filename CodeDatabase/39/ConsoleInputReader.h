#pragma once

#include <iostream>
#include "Command.h"
#include "KeyBindings.h"

class ConsoleInputReader {
public:
    explicit ConsoleInputReader(const KeyBindings& bindings);

    Command ReadCommand();

private:
    KeyBindings bindings_;
};
