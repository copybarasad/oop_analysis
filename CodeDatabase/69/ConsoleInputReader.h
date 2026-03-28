#ifndef CONSOLE_INPUT_READER_H
#define CONSOLE_INPUT_READER_H

#include "Commands/Command.h"
#include <memory>
#include <string>
#include <map>

enum class CommandType {
    Move,
    Cast,
    Buy,
    Skip,
    Save,
    Load,
    Quit,
    Unknown
};

class ConsoleInputReader {
public:
    ConsoleInputReader();
    
    void LoadConfig(const std::string& filename);
    std::unique_ptr<Command> ReadCommand();
    
    int ReadInt();
    char ReadChar();
    std::string ReadString();

private:
    void PrintHelp() const;
    std::map<std::string, CommandType> keyMap_;
    
    // Reverse map for help display: CommandType -> Key
    std::string GetKeyFor(CommandType type) const;
};

#endif
