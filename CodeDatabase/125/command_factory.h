#pragma once
#include "command.h"
#include "input_config.h"
#include <memory>

class CommandFactory {
public:
    static void SetConfig(const InputConfig& config);
    
    static std::unique_ptr<Command> CreateCommandFromInput(char input);
    
    static std::unique_ptr<Command> CreateCommandFromType(CommandType type, int param = 0);
    
private:
    static InputConfig config_;
};
