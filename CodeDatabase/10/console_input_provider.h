#pragma once
#include "input_provider.h"
#include <memory>
#include <string>

class Command;

class ConsoleInputProvider : public InputProvider {
private:
    std::string lastInput;
    
public:
    ConsoleInputProvider() = default;
    std::unique_ptr<Command> getNextCommand() override;
    void showPrompt(const std::string& prompt) override;
};
