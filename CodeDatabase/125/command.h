#pragma once
#include <string>
#include <memory>

class GameSession;

class Command {
public:
    virtual ~Command() = default;
    virtual void Execute(GameSession& game) = 0;
    virtual std::string GetDescription() const = 0;
    virtual bool IsValid() const { return true; }
};
