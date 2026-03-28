#ifndef COMMAND_H
#define COMMAND_H

#include <utility>

enum class CommandType {
    None,
    Move,
    ToggleAttackMode, 
    SelectCard,
    BuySpell,
    CastSpellAtCell,
    SaveGame,
    LoadGame
};

class Command {
public:
    CommandType type{CommandType::None};

    int deltaX = 0;
    int deltaY = 0;

    int index = -1;

    bool hasTarget = false;
    std::pair<int,int> targetCell{0, 0};

    static Command Move(int dx, int dy) {
        Command c;
        c.type = CommandType::Move;
        c.deltaX = dx;
        c.deltaY = dy;
        return c;
    }

    static Command ToggleAttackMode() {
        Command c;
        c.type = CommandType::ToggleAttackMode;
        return c;
    }

    static Command SelectCard(int index) {
        Command c;
        c.type = CommandType::SelectCard;
        c.index = index;
        return c;
    }

    static Command BuySpell() {
        Command c;
        c.type = CommandType::BuySpell;
        return c;
    }

    static Command CastSpell(std::pair<int,int> cell) {
        Command c;
        c.type = CommandType::CastSpellAtCell;
        c.hasTarget = true;
        c.targetCell = cell;
        return c;
    }

    static Command SaveGame() {
        Command c;
        c.type = CommandType::SaveGame;
        return c;
    }

    static Command LoadGame() {
        Command c;
        c.type = CommandType::LoadGame;
        return c;
    }
};



#endif