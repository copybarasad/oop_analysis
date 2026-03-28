#ifndef COMMAND_H
#define COMMAND_H

#include <variant>
#include <string>

struct MoveCommand {
    int dx, dy;
};

struct AttackCommand {
    int dx, dy;
};

struct UseSpellCommand {
    int spellIndex;
    int x, y;
};

struct SwitchModeCommand {};
struct BuySpellCommand {};
struct SaveGameCommand { std::string filename; };
struct ExitCommand {};

using Command = std::variant<
    MoveCommand,
    AttackCommand,
    UseSpellCommand,
    SwitchModeCommand,
    BuySpellCommand,
    SaveGameCommand,
    ExitCommand,
    std::monostate
>;

inline bool isExit(const Command& cmd) {
    return std::holds_alternative<ExitCommand>(cmd);
}

inline bool isInvalid(const Command& cmd) {
    return std::holds_alternative<std::monostate>(cmd);
}

#endif