#pragma once

#include "Command.h"
#include "MenuInput.h"
#include "KeyBoardInput.h"
#include <optional>

class FullGameInput {
public:
    FullGameInput() : mode_(Mode::Menu) {}
    std::optional<Command> getNextCommand();
    void switchToGameplay();
    void switchToMenu();

private:
    enum class Mode{
        Menu,
        Gameplay
    }
    
    mode_ = Mode::Menu;
    MenuInput menu_;
    KeyboardInput gameplay_;
};