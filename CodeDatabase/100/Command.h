#pragma once
#include <memory>
#include <string>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual std::string getName() const = 0;
};