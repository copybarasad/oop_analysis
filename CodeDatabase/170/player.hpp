#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"
#include "player-hand.hpp"

namespace game
{
  class Player final: public Entity
  {
  public:
    Player(PlayerTraits traits, Field* field_ptr, Position pos);
    Player(const Player& rhs);

    void use_spell_card(size_t index, Position pos);
    void add_boost_spell_card();
    void add_area_spell_card();
    void add_trap_spell_card();

    size_t get_score() const noexcept;
    void gain_points(size_t amount) noexcept;
    void spend_points(size_t amount) noexcept;

    PlayerTraits get_traits() const;
    PlayerHand& get_hand();

  private:
    size_t score_;
    PlayerHand hand_;
  };
}

#endif
