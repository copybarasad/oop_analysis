#include "MoveCommand.h"

MoveCommand::MoveCommand(int deltaX, int deltaY) : dx(deltaX), dy(deltaY) { }

void MoveCommand::execute() {}

std::string MoveCommand::getName() const { return "Move"; }

int MoveCommand::getDX() const { return dx; }
int MoveCommand::getDY() const { return dy; }