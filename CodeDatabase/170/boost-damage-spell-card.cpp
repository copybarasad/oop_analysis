#include "boost-damage-spell-card.hpp"
#include "field.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy-tower.hpp"

game::BoostDamageSpellCard::BoostDamageSpellCard(size_t spell_damage):
  SpellCard(spell_damage)
{}

void game::BoostDamageSpellCard::cast(Field& field, Position pos)
{
  if (field.get_square_type(pos) == SquareType::Enemy)
  {
    Enemy* enemy = field.get_enemy(pos);
    if (enemy != nullptr)
    {
      enemy->take_damage(spell_damage_);
    }
  }
  else if (field.get_square_type(pos) == SquareType::EnemyTower)
  {
    EnemyTower* tower = field.get_tower(pos);
    if (tower != nullptr)
    {
      tower->take_damage(spell_damage_);
    }
  }
  else if (field.get_square_type(pos) == SquareType::Player)
  {
    Player& player = field.get_player();
    if (player.get_position() == pos)
    {
      player.take_damage(spell_damage_);
    }
  }
}
