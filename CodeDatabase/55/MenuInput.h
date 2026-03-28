#pragma once

#include "Command.h"
#include <optional>
#include <iostream>

class MenuInput {
public:
    std::optional<Command> getNextCommand();
    void reset();
    MenuInput() : state_(State::MainMenu), firstMainMenu_(true), firstLevelsMenu_(true){}
    
private:

    enum class State {
        MainMenu,
        LevelsMenu,
        UpgradeMenu,
        HarpoonSubmenu,
        SpellSubmenu
    };

    State state_ = State::MainMenu;

    bool firstMainMenu_ = true;
    bool firstLevelsMenu_ = true;

    bool safeInputChar(char& out);
    bool safeInputInt(int& out);

    std::optional<Command> handleMainMenu();
    std::optional<Command> handleLevelsMenu();
};