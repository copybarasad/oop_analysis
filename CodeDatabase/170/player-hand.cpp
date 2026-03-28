#include "player-hand.hpp"
#include <stdexcept>
#include <random>

game::PlayerHand::PlayerHand(Player* owner_ptr, unsigned short capacity):
  capacity_(capacity),
  owner_ptr_(owner_ptr)
{}

game::SpellCard* game::PlayerHand::get_spell_card(size_t index)
{
  if (index >= spell_cards_.size())
  {
    throw std::out_of_range("Index is out of range");
  }
  return spell_cards_[index].get();
}

void game::PlayerHand::add_spell_card(std::unique_ptr< SpellCard > spell_card)
{
  if (spell_cards_.size() >= capacity_)
  {
    throw std::overflow_error("Player hand capacity overflow");
  }
  spell_cards_.emplace_back(std::move(spell_card));
}

void game::PlayerHand::remove_spell_card(size_t index)
{
  if (index < spell_cards_.size())
  {
    spell_cards_.erase(spell_cards_.begin() + index);
  }
}

size_t game::PlayerHand::get_card_count() const noexcept
{
  return spell_cards_.size();
}

void game::PlayerHand::set_cards(std::vector< std::unique_ptr< SpellCard > > cards)
{
  spell_cards_ = std::move(cards);
  if (spell_cards_.size() > capacity_)
  {
    spell_cards_.resize(capacity_);
  }
}

void game::PlayerHand::remove_random_half_cards()
{
  size_t count_to_remove = spell_cards_.size() / 2;
  if (count_to_remove == 0)
  {
    return;
  }

  static std::mt19937 gen(std::random_device{}());
  for (size_t i = 0; i < count_to_remove; ++i)
  {
    std::uniform_int_distribution<> dist(0, static_cast<int>(spell_cards_.size()) - 1);
    size_t index_to_remove = dist(gen);
    remove_spell_card(index_to_remove);
  }
}

std::vector< std::unique_ptr< game::SpellCard > > game::PlayerHand::get_cards_for_transfer()
{
  return std::move(spell_cards_);
}
