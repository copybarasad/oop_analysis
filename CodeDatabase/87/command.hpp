#ifndef COMMAND_HPP
#define COMMAND_HPP

class Game_controller;
class Command{
    public:
        virtual ~Command() = default;
        virtual void execute(Game_controller& game) = 0;
};

#endif