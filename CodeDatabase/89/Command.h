#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <variant>
#include <memory>

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct MoveCommand {
    Direction direction;
};

struct AttackCommand {
    Direction direction;
};

struct CastSpellCommand {
    int spellIndex;
    Direction direction;
};

struct SwitchModeCommand {};

struct ViewSpellsCommand {};

struct SaveGameCommand {
    std::string filename;
};

struct LoadGameCommand {
    std::string filename;
};

struct QuitCommand {};
struct RestartLevelCommand {};
struct NextLevelCommand {};

class Command {
public:
    using CommandType = std::variant<
        MoveCommand,
        AttackCommand,
        CastSpellCommand,
        SwitchModeCommand,
        ViewSpellsCommand,
        SaveGameCommand,
        LoadGameCommand,
        QuitCommand,
        RestartLevelCommand,
        NextLevelCommand
    >;

    Command(CommandType cmd) : command(std::move(cmd)) {}

    template<typename T>
    bool is() const {
        return std::holds_alternative<T>(command);
    }

    template<typename T>
    const T& get() const {
        return std::get<T>(command);
    }

    // Helper methods for common commands
    bool isMove() const { return is<MoveCommand>(); }
    bool isAttack() const { return is<AttackCommand>(); }
    bool isCastSpell() const { return is<CastSpellCommand>(); }
    bool isSwitchMode() const { return is<SwitchModeCommand>(); }
    bool isViewSpells() const { return is<ViewSpellsCommand>(); }
    bool isSaveGame() const { return is<SaveGameCommand>(); }
    bool isLoadGame() const { return is<LoadGameCommand>(); }
    bool isQuit() const { return is<QuitCommand>(); }
    bool isRestartLevel() const { return is<RestartLevelCommand>(); }
    bool isNextLevel() const { return is<NextLevelCommand>(); }

private:
    CommandType command;
};

#endif