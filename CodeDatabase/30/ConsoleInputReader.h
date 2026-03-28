#pragma once
#include "InputReader.h"
#include <map>
#include <string>

class ConsoleInputReader : public InputReader {
public:
    ConsoleInputReader();
    std::unique_ptr<Command> readCommand(const GameCycle& game) override;

private:
    void loadConfig(const std::string& filename);
    void setDefaultConfig();

    std::map<std::string, std::string> commandToKey_;
    std::map<std::string, std::string> keyToCommand_;
};