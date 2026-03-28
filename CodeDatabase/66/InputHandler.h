#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "PlayerAction.h"

class SpellHand;

class InputHandler {
public:
    PlayerAction getPlayerAction(const SpellHand* spellHand = nullptr);
    
private:
    Direction parseDirectionInput();
};

#endif
