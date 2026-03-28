#include "../gui_headers/command.hpp"

KeyboardCommand::KeyboardCommand(CommandType cmdType) : type(cmdType){}

CommandType KeyboardCommand::getType() const{
    return type;
}

std::string KeyboardCommand::toString() const{
    switch(type) {
        case CommandType::MOVE_UP:
            return "MOVE_UP";
        case CommandType::MOVE_DOWN:
            return "MOVE_DOWN";
        case CommandType::MOVE_LEFT:
            return "MOVE_LEFT";
        case CommandType::MOVE_RIGHT:
            return "MOVE_RIGHT";
        case CommandType::TOGGLE_FIGHT:
            return "TOGGLE_FIGHT";
        case CommandType::SHOW_ATTACK_RADIUS:
            return "SHOW_ATTACK_RADIUS";
        case CommandType::BUY_SPELL:
            return "BUY_SPELL";
        case CommandType::SAVE_GAME:
            return "SAVE_GAME";
        case CommandType::CAST_SPELL_1:
            return "CAST_SPELL_1";
        case CommandType::CAST_SPELL_2:
            return "CAST_SPELL_2";
        case CommandType::CAST_SPELL_3:
            return "CAST_SPELL_3";
        case CommandType::CAST_SPELL_4:
            return "CAST_SPELL_4";
        case CommandType::CAST_SPELL_5:
            return "CAST_SPELL_5";
        case CommandType::CAST_SPELL_6:
            return "CAST_SPELL_6";
        case CommandType::CANCEL:
            return "CANCEL";
        case CommandType::NEW_GAME:
            return "NEW_GAME";
        case CommandType::CONTINUE_GAME:
            return "CONTINUE_GAME";
        case CommandType::EXIT:
            return "EXIT";
        case CommandType::CONTINUE_NEXT_LEVEL:
            return "CONTINUE_NEXT_LEVEL";
        case CommandType::MAIN_MENU:
            return "MAIN_MENU";
        case CommandType::UPGRADE_1:
            return "UPGRADE_1";
        case CommandType::UPGRADE_2:
            return "UPGRADE_2";
        case CommandType::UPGRADE_3:
            return "UPGRADE_3";
        case CommandType::UPGRADE_4:
            return "UPGRADE_4";
        case CommandType::BUY_SPELL_1:
            return "BUY_SPELL_1";
        case CommandType::BUY_SPELL_2:
            return "BUY_SPELL_2";
        case CommandType::BUY_SPELL_3:
            return "BUY_SPELL_3";
        case CommandType::BUY_SPELL_4:
            return "BUY_SPELL_4";
        case CommandType::BUY_SPELL_5:
            return "BUY_SPELL_5";
        case CommandType::MOUSE_CLICK:
            return "MOUSE_CLICK";
        default:
            return "UNKNOWN";
    }
}

MouseCommand::MouseCommand(const sf::Vector2i& pos) : position(pos){}

CommandType MouseCommand::getType() const{
    return CommandType::MOUSE_CLICK;
}

std::string MouseCommand::toString() const{
    return "MOUSE_CLICK at ("+std::to_string(position.x)+", "+std::to_string(position.y) + ")";
}

sf::Vector2i MouseCommand::getPosition() const{
    return position;
}