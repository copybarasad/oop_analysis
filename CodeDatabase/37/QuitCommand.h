#ifndef QUITCOMMAND_H
#define QUITCOMMAND_H

#include "Command.h"

class QuitCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override { return "Quit"; }
    Command* clone() const override { return new QuitCommand(*this); }
};

#endif