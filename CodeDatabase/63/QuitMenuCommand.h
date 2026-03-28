#ifndef RPG_QUITMENUCOMMAND_H
#define RPG_QUITMENUCOMMAND_H

#include "Command.h"

class QuitMenuCommand : public Command {
    bool& running;
public:
    explicit QuitMenuCommand(bool& running) : running(running) {}

    std::pair<bool, std::string> execute() override {
        running = false;
        return {false, "Программа завершена\n"};
    }
};


#endif