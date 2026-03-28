#include "trap_spell.h"
#include "../map/game_map.h"

TrapSpell::TrapSpell()
    : name("Trap"), mana_cost(COST), cooldown(0), 
      max_cooldown(DEFAULT_COOLDOWN), damage(DAMAGE), cast_range(CAST_RANGE) {}

int TrapSpell::get_mana_cost() const { return mana_cost; }
std::string TrapSpell::get_name() const { return name; }
int TrapSpell::get_cooldown() const { return cooldown; }
int TrapSpell::get_max_cooldown() const { return max_cooldown; }
int TrapSpell::get_damage() const { return damage; }
int TrapSpell::get_cast_range() const { return cast_range; }

void TrapSpell::set_cooldown(int value)
{
    cooldown = std::clamp(value, 0, max_cooldown);
}

bool TrapSpell::is_available(int player_mana) const
{
    return cooldown == 0 && player_mana >= mana_cost;
}

void TrapSpell::update_cooldown()
{
    if (cooldown > 0)
    {
        cooldown--;
    }
}

void TrapSpell::cast(GameMap& map, int target_x, int target_y)
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
    
    if (cell_type == CellType::EMPTY)
    {
        target_cell.set_trap_spell(true);
        player->subtract_mana(get_mana_cost());
        cooldown = max_cooldown;
    }
}

void TrapSpell::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&cooldown), sizeof(cooldown));
}

void TrapSpell::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&cooldown), sizeof(cooldown));
}