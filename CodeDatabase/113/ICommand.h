#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>
#include <memory>

class GameSession;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(GameSession& session) = 0;
    virtual std::string getDescription() const = 0;
};

#endif // ICOMMAND_H