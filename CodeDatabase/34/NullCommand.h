#ifndef GAME_NULLCOMMAND_H
#define GAME_NULLCOMMAND_H

#include "ICommand.h"

namespace Game {

// Команда-пустышка для невалидного ввода
class NullCommand : public ICommand {
public:
    void execute(Game& game) override;
};

}

#endif
