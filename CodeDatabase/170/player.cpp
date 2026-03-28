#include "player.hpp"
#include <cmath>
#include <stdexcept>
#include <random>
#include "boost-damage-spell-card.hpp"
#include "area-damage-spell-card.hpp"
#include "trap-spell-card.hpp"

game::Player::Player(PlayerTraits traits, Field* field_ptr, Position pos):
  Entity(traits.base_traits, field_ptr, pos),
  score_(traits.score),
  hand_(this)
{
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<> dist(0, 2);
  int spell_type = dist(gen);

  if (spell_type == 0)
  {
    add_boost_spell_card();
  }
  else if (spell_type == 1)
  {
    add_area_spell_card();
  }
  else
  {
    add_trap_spell_card();
  }
}

game::Player::Player(const Player& rhs):
  Entity(rhs),
  hand_(this)
{}

void game::Player::gain_points(size_t amount) noexcept
{
  score_ += amount;
}

void game::Player::spend_points(size_t amount) noexcept
{
  score_ = amount > score_ ? 0 : score_ - amount;
}

void game::Player::add_boost_spell_card()
{
  hand_.add_spell_card(std::make_unique< BoostDamageSpellCard >(5));
}

void game::Player::add_area_spell_card()
{
  hand_.add_spell_card(std::make_unique< AreaDamageSpellCard >(10));
}

void game::Player::add_trap_spell_card()
{
  hand_.add_spell_card(std::make_unique< TrapSpellCard >(3));
}

void game::Player::use_spell_card(size_t index, Position pos)
{
  Position player_pos = get_position();
  int distance_x = std::abs(static_cast< int >(player_pos.first) - static_cast< int >(pos.first));
  int distance_y = std::abs(static_cast< int >(player_pos.second) - static_cast< int >(pos.second));
  if (distance_x > 5 || distance_y > 5) {
    throw std::runtime_error("Target position is too far for spell casting");
  }

  SpellCard* card = hand_.get_spell_card(index);
  card->cast(*field_ptr_, pos);
  hand_.remove_spell_card(index);
}

size_t game::Player::get_score() const noexcept
{
  return score_;
}

game::PlayerTraits game::Player::get_traits() const
{
  PlayerTraits traits;
  traits.base_traits.health_points = health_points_;
  traits.base_traits.attack_damage = attack_damage_;
  traits.score = score_;
  return traits;
}

game::PlayerHand& game::Player::get_hand()
{
  return hand_;
}
