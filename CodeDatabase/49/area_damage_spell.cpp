#include "area_damage_spell.h"
#include "../map/game_map.h"
#include "../enemies/enemy.h"
#include "../enemies/enemy_building.h"

AreaDamageSpell::AreaDamageSpell()
    : name("Area Damage"), mana_cost(COST), cooldown(0), 
      max_cooldown(DEFAULT_COOLDOWN), damage(DAMAGE), 
      cast_range(CAST_RANGE), area_size(AREA_SIZE) {}

int AreaDamageSpell::get_mana_cost() const { return mana_cost; }
std::string AreaDamageSpell::get_name() const { return name; }
int AreaDamageSpell::get_cooldown() const { return cooldown; }
int AreaDamageSpell::get_max_cooldown() const { return max_cooldown; }
int AreaDamageSpell::get_damage() const { return damage; }
int AreaDamageSpell::get_cast_range() const { return cast_range; }
int AreaDamageSpell::get_area_size() const { return area_size; }

void AreaDamageSpell::set_cooldown(int value)
{
    cooldown = std::clamp(value, 0, max_cooldown);
}

bool AreaDamageSpell::is_available(int player_mana) const
{
    return cooldown == 0 && player_mana >= mana_cost;
}

void AreaDamageSpell::update_cooldown()
{
    if (cooldown > 0)
    {
        cooldown--;
    }
}

void AreaDamageSpell::cast(GameMap& map, int target_x, int target_y)
{
    Player* player = map.get_player();
    
    int player_x = player->get_x();
    int player_y = player->get_y();
    int distance = std::abs(target_x - player_x) + std::abs(target_y - player_y);
    
    if (distance > cast_range)
    {
        return;
    }
    
    bool spell_used = false;

    int radius = area_size;

    for (int dy = -radius; dy <= radius; ++dy)
    {
        for (int dx = -radius; dx <= radius; ++dx)
        {
            int x = target_x + dx;
            int y = target_y + dy;

            if (std::abs(dx) + std::abs(dy) > radius)
                continue;

            if (!map.is_valid_position(x, y))
                continue;

            Cell& target_cell = map.get_cell(x, y);
            CellType cell_type = target_cell.get_type();

            if (cell_type == CellType::ENEMY)
            {
                for (Enemy& enemy : map.get_enemies_mutable())
                {
                    if (enemy.is_alive() && enemy.get_x() == x && enemy.get_y() == y)
                    {
                        enemy.take_damage(damage);
                        spell_used = true;

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
                    building->get_x() == x && building->get_y() == y)
                {
                    building->take_damage(damage);
                    spell_used = true;
                    
                    if (!building->is_alive())
                    {
                        target_cell.set_type(CellType::EMPTY);
                        target_cell.set_availability(true);
                        player->add_score(SCORE_PER_DESTROY_BUILDING);
                    }
                }
            }
            else if (cell_type == CellType::EMPTY)
            {
                spell_used = true;
            }
        }
    }
    
    if (spell_used)
    {
        player->subtract_mana(get_mana_cost());
        cooldown = max_cooldown;
    }
}

void AreaDamageSpell::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&cooldown), sizeof(cooldown));
}

void AreaDamageSpell::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&cooldown), sizeof(cooldown));
}