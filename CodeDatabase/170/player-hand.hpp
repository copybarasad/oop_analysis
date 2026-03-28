#ifndef PLAYER_HAND_HPP
#define PLAYER_HAND_HPP

#include <vector>
#include <memory>
#include "spell-card.hpp"

namespace game
{
  class Player;

  constexpr unsigned short INITIAL_PLAYER_HAND_CAPACITY = 5;
  class PlayerHand
  {
  public:
    PlayerHand(Player* owner_ptr, unsigned short capacity = INITIAL_PLAYER_HAND_CAPACITY);

    SpellCard* get_spell_card(size_t index);
    void add_spell_card(std::unique_ptr< SpellCard > spell_card);
    void remove_spell_card(size_t index);

    size_t get_card_count() const noexcept;
    void set_cards(std::vector< std::unique_ptr< SpellCard > > cards);
    void remove_random_half_cards();
    std::vector< std::unique_ptr< SpellCard > > get_cards_for_transfer();

  private:
    std::vector< std::unique_ptr< SpellCard > > spell_cards_;
    size_t capacity_;
    Player* owner_ptr_;
  };
}

#endif
