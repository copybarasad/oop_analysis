#ifndef CONSOLE_INPUT_READER_H
#define CONSOLE_INPUT_READER_H

#include "input_reader.h"
#include <map>
#include <string>

class ConsoleInputReader : public InputReader {
public:
    ConsoleInputReader();
    Command readCommand() override;
    void loadKeyBindings(const std::string& filename);
    
private:
    std::map<char, Command::Type> keyBindings;
    void setDefaultBindings();
    char readChar() const;
};

#endif