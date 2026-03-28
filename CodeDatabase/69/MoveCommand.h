#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "Command.h"

class MoveCommand : public Command {
 public:
  MoveCommand(int dx, int dy);
  CommandResult execute(Game& game) override;
  bool EndsTurn() const override { return true; }

 private:
  int dx_;
  int dy_;
};

#endif
