#ifndef COMMAND_H
#define COMMAND_H

enum class PlayerAction {
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_COMBAT_MODE,
    CAST_SPELL,
    SKIP_TURN,
    ATTACK,
    SAVE_GAME,
    LOAD_GAME,
    PAUSE_GAME,
    RESUME_GAME,
    START_NEW_GAME,
    QUIT_GAME,
    SELECT_UPGRADE_1,
    SELECT_UPGRADE_2,
    SELECT_UPGRADE_3
};

class Command {
public:
    Command();
    explicit Command(PlayerAction action);
    Command(PlayerAction action, int spell_index, int target_x, int target_y);

    PlayerAction GetAction() const;
    int GetSpellIndex() const;
    int GetTargetX() const;
    int GetTargetY() const;
    bool IsValid() const;

    void SetAction(PlayerAction action);
    void SetSpellIndex(int index);
    void SetTarget(int x, int y);

private:
    PlayerAction action_;
    int spell_index_;
    int target_x_;
    int target_y_;
};

#endif // COMMAND_H
