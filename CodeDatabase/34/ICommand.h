#ifndef GAME_ICOMMAND_H
#define GAME_ICOMMAND_H

namespace Game {

class Game; 

// Интерфейс для всех команд
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Game& game) = 0;
};

}

#endif
