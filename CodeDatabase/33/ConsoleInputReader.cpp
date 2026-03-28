#include "ConsoleInputReader.h"
#include <cctype>
#include <sstream>

ConsoleInputReader::ConsoleInputReader(std::istream& input, const ControlBindings& bindings)
    : in_(input), bindings_(bindings) {}

Command ConsoleInputReader::ReadCommand() {
    std::string line;
    if (!std::getline(in_, line)) {
        return Command::MakeQuit();
    }
    if (line.empty()) return Command{};

    if (line[0] == ':') {
        std::istringstream iss(line.substr(1));
        std::string cmd;
        std::string arg;
        iss >> cmd >> arg;
        if (cmd == "save") {
            return Command::MakeSave(arg);
        }
        if (cmd == "load") {
            return Command::MakeLoad(arg);
        }
        return Command{};
    }

    char key = static_cast<char>(std::tolower(static_cast<unsigned char>(line[0])));
    std::string payload;
    if (line.size() > 1 && std::isspace(static_cast<unsigned char>(line[1]))) {
        payload = line.substr(2);
    }
    return bindings_.MapInput(key, payload);
}
