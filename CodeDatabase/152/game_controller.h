#pragma once
#include "command.h"
#include "action_type.h"
#include "action.h"
#include "game.h"

template <typename InputReader>
class GameController {
public:
    GameController(Game& game) : game_(game), input_() {}

    void loop() {
        Command cmd = input_.read();
        PlayerAction action = convert(cmd);
        game_.execute(action);
    }

private:
    Game& game_;
    InputReader input_;

    PlayerAction convert(const Command& cmd) {
        PlayerAction action;

        switch (cmd.type) {
        case CommandType::Move:
            action.type = ActionType::Move;
            switch (cmd.dir) {
            case 'w': case 'W': action.dr = -1; action.dc = 0; break;
            case 's': case 'S': action.dr = 1;  action.dc = 0; break;
            case 'a': case 'A': action.dr = 0;  action.dc = -1; break;
            case 'd': case 'D': action.dr = 0;  action.dc = 1; break;
            }
            break;

        case CommandType::Attack:
            action.type = ActionType::Attack;
            action.target_r = cmd.r;
            action.target_c = cmd.c;
            break;

        case CommandType::Spell:
            action.type = ActionType::CastSpell;
            action.spell_index = cmd.spell_index;
            action.target_r = cmd.r;
            action.target_c = cmd.c;
            break;

        case CommandType::ShowSpells:
            action.type = ActionType::ShowSpells;
            break;

        case CommandType::Save:
            action.type = ActionType::Save;
            action.filename = cmd.filename;
            break;

        case CommandType::Load:
            action.type = ActionType::Load;
            action.filename = cmd.filename;
            break;

        default:
            action.type = ActionType::None;
            break;
        }

        return action;
    }
};
