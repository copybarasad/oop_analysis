#ifndef OOP_COMMAND_TYPES_H_
#define OOP_COMMAND_TYPES_H_

namespace rpg {
    enum class CommandType {
        kMoveUp,
        kMoveDown,
        kMoveLeft,
        kMoveRight,
        kAttack,
        kSwitchMode,
        kBuySpell,
        kCastSpell0,
        kCastSpell1,
        kCastSpell2,
        kCastSpell3,
        kCastSpell4,
        kSaveGame,
        kQuit,
        kInvalid
    };
}

#endif

