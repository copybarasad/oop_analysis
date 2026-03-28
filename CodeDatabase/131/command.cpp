#include "command.h"
#include "game.h"

namespace rpg {
    MoveCommand::MoveCommand(const Direction direction) : direction_(direction) {}
    
    CommandType MoveCommand::GetType() const {
        switch (direction_) {
            case Direction::kUp: return CommandType::kMoveUp;
            case Direction::kDown: return CommandType::kMoveDown;
            case Direction::kLeft: return CommandType::kMoveLeft;
            case Direction::kRight: return CommandType::kMoveRight;
            default: return CommandType::kInvalid;
        }
    }
    
    CommandType CastSpellCommand::GetType() const {
        switch (spell_index_) {
            case 0: return CommandType::kCastSpell0;
            case 1: return CommandType::kCastSpell1;
            case 2: return CommandType::kCastSpell2;
            case 3: return CommandType::kCastSpell3;
            case 4: return CommandType::kCastSpell4;
            default: return CommandType::kInvalid;
        }
    }
}
