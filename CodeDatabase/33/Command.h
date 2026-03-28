#pragma once
#include <string>

enum class CommandType {
    Invalid,
    Move,
    Attack,
    ToggleMode,
    CastSpell,
    Save,
    Load,
    Quit
};

struct Command {
    CommandType type = CommandType::Invalid;
    int dx = 0;
    int dy = 0;
    int spell_index = -1;
    std::string argument;

    static Command MakeMove(int dx, int dy) {
        Command cmd;
        cmd.type = CommandType::Move;
        cmd.dx = dx;
        cmd.dy = dy;
        return cmd;
    }

    static Command MakeAttack(int dx, int dy) {
        Command cmd;
        cmd.type = CommandType::Attack;
        cmd.dx = dx;
        cmd.dy = dy;
        return cmd;
    }

    static Command MakeToggle() {
        Command cmd;
        cmd.type = CommandType::ToggleMode;
        return cmd;
    }

    static Command MakeCast(int index) {
        Command cmd;
        cmd.type = CommandType::CastSpell;
        cmd.spell_index = index;
        return cmd;
    }

    static Command MakeSave(const std::string& path) {
        Command cmd;
        cmd.type = CommandType::Save;
        cmd.argument = path;
        return cmd;
    }

    static Command MakeLoad(const std::string& path) {
        Command cmd;
        cmd.type = CommandType::Load;
        cmd.argument = path;
        return cmd;
    }

    static Command MakeQuit() {
        Command cmd;
        cmd.type = CommandType::Quit;
        return cmd;
    }
};
