#include "command.hpp"

Command::Command(GameAction a) : action(a) {}
Command::Command(GameAction a, int idx) : action(a), spell_index(idx) {}
Command::Command(GameAction a, int tx, int ty) : action(a), target_x(tx), target_y(ty) {}
Command::Command(GameAction a, int idx, int tx, int ty) : action(a), spell_index(idx), target_x(tx), target_y(ty) {}
Command::Command(GameAction a, const std::string& name) : action(a), save_filename(name) {}