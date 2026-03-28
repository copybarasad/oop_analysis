#include "direct_damage_spell.h"
#include "../map/game_map.h"
#include "../enemies/enemy.h"
#include "../enemies/enemy_building.h"

DirectDamageSpell::DirectDamageSpell()
    : name("Direct Damage"), mana_cost(COST), cooldown(0), 
      max_cooldown(DEFAULT_COOLDOWN), damage(DAMAGE), cast_range(CAST_RANGE) {}

int DirectDamageSpell::get_mana_cost() const { return mana_cost; }
std::string DirectDamageSpell::get_name() const { return name; }
int DirectDamageSpell::get_cooldown() const { return cooldown; }
int DirectDamageSpell::get_max_cooldown() const { return max_cooldown; }
int DirectDamageSpell::get_damage() const { return damage; }
int DirectDamageSpell::get_cast_range() const { return cast_range; }

void DirectDamageSpell::set_cooldown(int value)
{
    cooldown = std::clamp(value, 0, max_cooldown);
}

bool DirectDamageSpell::is_available(int player_mana) const
{
    return cooldown == 0 && player_mana >= mana_cost;
}

void DirectDamageSpell::update_cooldown()
{
    if (cooldown > 0)
    {
        cooldown--;
    }
}

void DirectDamageSpell::cast(GameMap& map, int target_x, int target_y)
{
    Player* player = map.get_player();
    
    int player_x = player->get_x();
    int player_y = player->get_y();
    int distance = std::abs(target_x - player_x) + std::abs(target_y - player_y);
    
    if (distance > cast_range)
    {
        return;
    }
    
    Cell& target_cell = map.get_cell(target_x, target_y);
    CellType cell_type = target_cell.get_type();
    
    if (cell_type == CellType::ENEMY)
    {
        for (Enemy& enemy : map.get_enemies_mutable())
        {
            if (enemy.is_alive() && enemy.get_x() == target_x && enemy.get_y() == target_y)
            {
                enemy.take_damage(damage);
                player->subtract_mana(get_mana_cost());
                cooldown = max_cooldown;
                
                if (!enemy.is_alive())
                {
                    target_cell.set_type(CellType::EMPTY);
                    target_cell.set_availability(true);
                    player->add_score(Player::SCORE_PER_KILL);
                }
                break;
            }
        }
    }
    else if (cell_type == CellType::ENEMY_BUILDING)
    {
        EnemyBuilding* building = map.get_enemy_building();
        if (building && building->is_alive() && 
            building->get_x() == target_x && building->get_y() == target_y)
        {
            building->take_damage(damage);
            player->subtract_mana(get_mana_cost());
            cooldown = max_cooldown;
            
            if (!building->is_alive())
            {
                target_cell.set_type(CellType::EMPTY);
                target_cell.set_availability(true);
                player->add_score(SCORE_PER_DESTROY_BUILDING);
            }
        }
    }
    else if (cell_type == CellType::PLAYER)
    {
        if (player && player->is_alive() && 
            player->get_x() == target_x && player->get_y() == target_y)
        {
            player->take_damage(TOWER_DAMAGE);
            cooldown = max_cooldown;
        }
    }
}

void DirectDamageSpell::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&cooldown), sizeof(cooldown));
}

void DirectDamageSpell::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&cooldown), sizeof(cooldown));
}