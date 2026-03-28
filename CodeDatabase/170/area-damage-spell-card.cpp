#include "area-damage-spell-card.hpp"
#include "field.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy-tower.hpp"

game::AreaDamageSpellCard::AreaDamageSpellCard(size_t spell_damage):
  SpellCard(spell_damage)
{}

void game::AreaDamageSpellCard::cast(Field& field, Position pos)
{
  for (size_t x = pos.first; x <= pos.first + 1 && x < field.get_grid_size().first; ++x)
  {
    for (size_t y = pos.second; y <= pos.second + 1 && y < field.get_grid_size().second; ++y)
    {
      Position curr_pos = { x, y };
      if (field.is_valid_position(curr_pos))
      {
        if (field.get_square_type(curr_pos) == SquareType::Enemy)
        {
          Enemy* enemy = field.get_enemy(curr_pos);
          if (enemy) {
            enemy->take_damage(spell_damage_);
          }
        }
        else if (field.get_square_type(curr_pos) == SquareType::EnemyTower)
        {
          EnemyTower* tower = field.get_tower(curr_pos);
          if (tower) {
            tower->take_damage(spell_damage_);
          }
        }
      }
    }
  }
}
