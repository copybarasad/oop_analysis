#pragma once
#include <istream>
#include <string>
#include "ControlBindings.h"
#include "Command.h"

class ConsoleInputReader {
public:
    ConsoleInputReader(std::istream& input, const ControlBindings& bindings);

    Command ReadCommand();

private:
    std::istream& in_;
    ControlBindings bindings_;
};
