#pragma once

#include <unordered_map>
#include <string>

enum class Command {
    NONE,

    MENU_NEW_GAME,
    MENU_LOAD_GAME,
    MENU_EXIT,

    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,

    ATTACK,
    TARGET_MODE,
    CHANGE_SPELL,
    SAVE_GAME,
    EXIT_TO_MENU
};

class InputHandler {
public:
    InputHandler(const std::string& configFile = "controls.cfg");

    Command processInput();

private:
    std::unordered_map<char, Command> keymap;

    void loadConfig(const std::string& filename);
    Command commandFromString(const std::string& str);
    void setDefaultControls();

    static const std::vector<Command> requiredCommands;
};
