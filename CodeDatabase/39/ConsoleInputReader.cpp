#include "ConsoleInputReader.h"
#include <cctype>

ConsoleInputReader::ConsoleInputReader(const KeyBindings& bindings)
    : bindings_(bindings) {}

Command ConsoleInputReader::ReadCommand() {
    char ch;
    if (!(std::cin >> ch)) {
        return {CommandType::None};
    }
    CommandType type = bindings_.GetCommandForKey(ch);
    return {type};
}
