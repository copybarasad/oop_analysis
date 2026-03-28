#include "command.h"

Command::Command() : type_(Type::INVALID), param_(0) {}

Command::Command(Type type) : type_(type), param_(0) {}

Command::Command(Type type, int param) : type_(type), param_(param) {}

Command::Type Command::getType() const { return type_; }

int Command::getParam() const { return param_; }

bool Command::isValid() const { return type_ != Type::INVALID; }

std::string Command::getName() const {
    switch (type_) {
        case Type::MOVE_UP: return "move_up";
        case Type::MOVE_DOWN: return "move_down";
        case Type::MOVE_LEFT: return "move_left";
        case Type::MOVE_RIGHT: return "move_right";
        case Type::SWITCH_MODE: return "switch_mode";
        case Type::RANGED_ATTACK: return "ranged_attack";
        case Type::CAST_SPELL_1: return "cast_1";
        case Type::CAST_SPELL_2: return "cast_2";
        case Type::CAST_SPELL_3: return "cast_3";
        case Type::CAST_SPELL_4: return "cast_4";
        case Type::CAST_SPELL_5: return "cast_5";
        case Type::SAVE_GAME: return "save";
        case Type::LOAD_GAME: return "load";
        case Type::RESTART_GAME: return "restart";
        case Type::QUIT_GAME: return "quit";
        case Type::INVALID: return "invalid";
    }
    return "unknown";
}