#ifndef GAMELOOP_MESSAGE_MOVE_HPP
#define GAMELOOP_MESSAGE_MOVE_HPP

#include "../../direction.hpp"
#include "message.hpp"

class MoveMessage : public Message {
  public:
    MoveMessage(Direction dir);
    void handle(Player *player) override;

  private:
    Direction m_Direction;
};

#endif /* GAMELOOP_MESSAGE_MOVE_HPP */
