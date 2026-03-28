#include "console_input.h"

#include "game.h"

#include <limits>

ConsoleInput::ConsoleInput() : in(&std::cin) {}
ConsoleInput::ConsoleInput(std::istream& input) : in(&input) {}

static void ClearBadInput(std::istream& in) {
    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Command ConsoleInput::ReadCommand(const Game& game) {
    if (!in) {
        return Command{CommandType::INVALID};
    }

    const GameState state = game.state();

    if (state == GameState::MAIN_MENU) {
        int choice = 0;
        if (!(*in >> choice)) {
            ClearBadInput(*in);
            return Command{CommandType::INVALID};
        }
        if (choice == 1) return Command{CommandType::START_NEW_GAME};
        if (choice == 2) return Command{CommandType::LOAD_GAME};
        if (choice == 3) return Command{CommandType::EXIT_GAME};
        return Command{CommandType::INVALID};
    }

    if (state == GameState::GAME_OVER) {
        int choice = 0;
        if (!(*in >> choice)) {
            ClearBadInput(*in);
            return Command{CommandType::EXIT_GAME};
        }
        if (choice == 1) return Command{CommandType::START_NEW_GAME};
        return Command{CommandType::EXIT_GAME};
    }

    if (state == GameState::LEVEL_COMPLETE) {
        if (game.level() >= 3) {
            int choice = 0;
            if (!(*in >> choice)) {
                ClearBadInput(*in);
                return Command{CommandType::EXIT_GAME};
            }
            if (choice == 1) return Command{CommandType::START_NEW_GAME};
            return Command{CommandType::EXIT_GAME};
        }

        int choice = 0;
        if (!(*in >> choice)) {
            ClearBadInput(*in);
            return Command{CommandType::LEVEL_UP_SKIP};
        }
        if (choice == 1) return Command{CommandType::LEVEL_UP_HP};
        if (choice == 2) return Command{CommandType::LEVEL_UP_DAMAGE};
        return Command{CommandType::LEVEL_UP_SKIP};
    }

    if (state != GameState::PLAYING) {
        return Command{CommandType::INVALID};
    }

    char cmd = 0;
    if (!(*in >> cmd)) {
        ClearBadInput(*in);
        return Command{CommandType::INVALID};
    }

    Command out;
    out.type = CommandType::INVALID;

    switch (cmd) {
    case 'w': out.type = CommandType::MOVE_UP; break;
    case 's': out.type = CommandType::MOVE_DOWN; break;
    case 'a': out.type = CommandType::MOVE_LEFT; break;
    case 'd': out.type = CommandType::MOVE_RIGHT; break;
    case 'f': out.type = CommandType::SWITCH_WEAPON; break;
    case 'v': out.type = CommandType::SAVE_GAME; break;
    case 'l': out.type = CommandType::LOAD_GAME; break;
    case 'q': out.type = CommandType::EXIT_GAME; break;

    case 'i': {
        out.type = CommandType::OPEN_SHOP;
        char ans = 'n';
        if (!(*in >> ans)) {
            ClearBadInput(*in);
            ans = 'n';
        }
        out.buy_spell = (ans == 'y' || ans == 'Y');
        break;
    }

    case 'c': {
        out.type = CommandType::CAST_SPELL;
        int idx = -1;
        int tx = -1;
        int ty = -1;
        if (!(*in >> idx >> tx >> ty)) {
            ClearBadInput(*in);
            return Command{CommandType::INVALID};
        }
        out.spell_index = idx;
        out.target_x = tx;
        out.target_y = ty;
        break;
    }

    default:
        out.type = CommandType::INVALID;
        break;
    }

    return out;
}
