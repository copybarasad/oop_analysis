#include "Command.h"

Command::Command()
    : action_(PlayerAction::NONE),
      spell_index_(-1),
      target_x_(-1),
      target_y_(-1) {}

Command::Command(PlayerAction action)
    : action_(action),
      spell_index_(-1),
      target_x_(-1),
      target_y_(-1) {}

Command::Command(PlayerAction action, int spell_index, int target_x, int target_y)
    : action_(action),
      spell_index_(spell_index),
      target_x_(target_x),
      target_y_(target_y) {}

PlayerAction Command::GetAction() const {
    return action_;
}

int Command::GetSpellIndex() const {
    return spell_index_;
}

int Command::GetTargetX() const {
    return target_x_;
}

int Command::GetTargetY() const {
    return target_y_;
}

bool Command::IsValid() const {
    return action_ != PlayerAction::NONE;
}

void Command::SetAction(PlayerAction action) {
    action_ = action;
}

void Command::SetSpellIndex(int index) {
    spell_index_ = index;
}

void Command::SetTarget(int x, int y) {
    target_x_ = x;
    target_y_ = y;
}
