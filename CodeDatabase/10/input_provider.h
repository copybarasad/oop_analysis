#pragma once
#include <memory>
#include <string>

class Command;

class InputProvider {
public:
    virtual ~InputProvider() = default;
    virtual std::unique_ptr<Command> getNextCommand() = 0;
    virtual void showPrompt(const std::string& prompt) = 0;
};
