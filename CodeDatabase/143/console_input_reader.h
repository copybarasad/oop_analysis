#pragma once

#include "move_direction.h"
#include "position.h"
#include "game_controller.h"

#include <string>

class ConsoleInputReader {
public:
    explicit ConsoleInputReader(GameController& ctrl);
    MoveDirection getMoveInput(bool& quitRequested);
    int chooseSpellIndex(size_t maxSpells) const;
    Position chooseTarget() const;
    int chooseShopItem(size_t maxItems) const;
    bool confirmQuitWithSave() const;
    void clearBuffer() const;

private:

    GameController& controller;
};