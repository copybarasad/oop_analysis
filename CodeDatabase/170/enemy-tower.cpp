#include "enemy-tower.hpp"
#include <cmath>
#include <ostream>
#include "field.hpp"
#include "player.hpp"
#include "boost-damage-spell-card.hpp"

game::EnemyTower::EnemyTower(EnemyTowerTraits traits, Field* field_ptr, Position pos):
  Entity(traits.base_traits, field_ptr, pos),
  attack_range_(traits.attack_range),
  max_cooldown_(traits.cooldown),
  current_cooldown_(0),
  spell_(std::make_unique<BoostDamageSpellCard>(traits.base_traits.attack_damage))
{}

game::EnemyTower::EnemyTower(const EnemyTower& rhs):
  Entity(rhs),
  attack_range_(rhs.attack_range_),
  max_cooldown_(rhs.max_cooldown_),
  current_cooldown_(rhs.current_cooldown_),
  spell_(std::make_unique<BoostDamageSpellCard>(rhs.get_attack_damage()))
{}

void game::EnemyTower::update(Player* player)
{
  if (current_cooldown_ > 0)
  {
    --current_cooldown_;
    return;
  }

  if (player == nullptr || !player->is_alive())
  {
    return;
  }

  Position player_pos = player->get_position();
  Position tower_pos = get_position();

  size_t dist_x = std::abs(static_cast<int>(player_pos.first) - static_cast<int>(tower_pos.first));
  size_t dist_y = std::abs(static_cast<int>(player_pos.second) - static_cast<int>(tower_pos.second));

  if (dist_x <= attack_range_ && dist_y <= attack_range_)
  {
    std::ostream& log_ = field_ptr_->get_log_stream();
    log_ << "Enemy Tower at (" << tower_pos.first << ',' << tower_pos.second << ") casts a spell on player.\n";
    spell_->cast(*field_ptr_, player_pos);
    log_ << "Player health: " << player->get_health_points() << "\n";

    current_cooldown_ = max_cooldown_;
  }
}

size_t game::EnemyTower::get_attack_range() const noexcept
{
  return attack_range_;
}

size_t game::EnemyTower::get_cooldown() const noexcept
{
  return max_cooldown_;
}
