#ifndef GAMELOOP_MESSAGE_SPELL_HPP
#define GAMELOOP_MESSAGE_SPELL_HPP

#include "../../direction.hpp"
#include "message.hpp"

class SpellMessage : public Message {
  public:
    SpellMessage(int index, Field *target);
    void handle(Player *player) override;

  private:
    int m_Index;
    Field *m_Target;
};

#endif /* GAMELOOP_MESSAGE_SPELL_HPP */
