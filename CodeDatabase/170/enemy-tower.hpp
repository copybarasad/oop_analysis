#ifndef ENEMY_TOWER_HPP
#define ENEMY_TOWER_HPP

#include "entity.hpp"
#include "player.hpp"
#include "spell-card.hpp"
#include <memory>

namespace game
{
  class Player;
  class Field;
  
  class EnemyTower final: public Entity
  {
  public:
    EnemyTower(EnemyTowerTraits traits, Field* field_ptr, Position pos);
    EnemyTower(const EnemyTower& rhs);

    void update(Player* player);

    size_t get_attack_range() const noexcept;
    size_t get_cooldown() const noexcept;

  private:
    size_t attack_range_;
    size_t max_cooldown_;
    size_t current_cooldown_;
    std::unique_ptr<SpellCard> spell_;
  };
}

#endif
