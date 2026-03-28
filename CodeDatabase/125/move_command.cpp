#include "move_command.h"
#include "game_session.h"

MoveCommand::MoveCommand(Direction direction) : direction_(direction) {}

void MoveCommand::Execute(GameSession& game) {
    Position new_pos = game.GetPlayer().GetPosition();
    
    switch (direction_) {
        case Direction::Up:    new_pos.y--; break;
        case Direction::Down:  new_pos.y++; break;
        case Direction::Left:  new_pos.x--; break;
        case Direction::Right: new_pos.x++; break;
    }
    
    game.ProcessMovement(new_pos);
}

std::string MoveCommand::GetDescription() const {
    switch (direction_) {
        case Direction::Up:    return "Движение вверх";
        case Direction::Down:  return "Движение вниз";
        case Direction::Left:  return "Движение влево";
        case Direction::Right: return "Движение вправо";
        default: return "Неизвестное направление";
    }
}
