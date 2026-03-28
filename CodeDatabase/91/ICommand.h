#ifndef COMMANDS_ICOMMAND_H
#define COMMANDS_ICOMMAND_H

class Game;

class ICommand {
    public:
        virtual ~ICommand() = default;
        virtual void execute(Game& game) = 0;
};

#endif