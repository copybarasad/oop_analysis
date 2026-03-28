#ifndef CONSOLE_INPUT_READER_H
#define CONSOLE_INPUT_READER_H

#include "input_reader.h"
#include <unordered_map>
#include <string>

class ConsoleInputReader : public IInputReader {
private:
    std::unordered_map<char, CommandType> keymap;

    void loadDefaultControls();
    void loadFromFile();

public:
    ConsoleInputReader();
    Command readCommand() override;
};

#endif
