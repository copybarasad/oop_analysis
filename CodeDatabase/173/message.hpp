#ifndef GAMELOOP_MESSAGE_HPP
#define GAMELOOP_MESSAGE_HPP

#include "../../player.hpp"

class Message {
  public:
    virtual void handle(Player *player) = 0;
};

#endif /* GAMELOOP_MESSAGE_HPP */
