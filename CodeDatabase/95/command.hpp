#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <optional>
#include <memory>

class Command {
public:
    virtual ~Command() = default;
};

enum class MenuAction {
    NONE,
    START_NEW_GAME,
    LOAD_GAME,
    SHOW_SAVES,
    EXIT,
    BACK,
    SELECT_SAVE,
    SAVE_GAME_NAME
};

struct MenuCommand : public Command {
    MenuAction action = MenuAction::NONE;
    std::optional<int> index;
    std::optional<std::string> payload;

    MenuCommand() = default;
    MenuCommand(MenuAction a) : action(a) {}
    MenuCommand(MenuAction a, int idx) : action(a), index(idx) {}
    MenuCommand(MenuAction a, std::string p) : action(a), payload(std::move(p)) {}
};

enum class GameAction {
    NONE,
    MOVE,
    CAST_SPELL,
    PAUSE
};

enum class MoveDir {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct GameCommand : public Command {
    GameAction action = GameAction::NONE;
    MoveDir dir = MoveDir::NONE;
    std::optional<size_t> spellIndex;
    std::optional<int> targetX;
    std::optional<int> targetY;

    GameCommand() = default;
    GameCommand(GameAction a) : action(a) {}

    static GameCommand Move(MoveDir d) {
        GameCommand c(GameAction::MOVE);
        c.dir = d;
        return c;
    }

    static GameCommand Pause() {
        return GameCommand(GameAction::PAUSE);
    }

    static GameCommand CastSpell(size_t idx) {
        GameCommand c(GameAction::CAST_SPELL);
        c.spellIndex = idx;
        return c;
    }

    static GameCommand CastSpellAt(size_t idx, int x, int y) {
        GameCommand c(GameAction::CAST_SPELL);
        c.spellIndex = idx;
        c.targetX = x;
        c.targetY = y;
        return c;
    }
};

#endif
