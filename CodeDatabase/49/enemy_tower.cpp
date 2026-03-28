#include "enemy_tower.h"

EnemyTower::EnemyTower(int start_x, int start_y)
    : x(start_x), y(start_y), attack_range(DEFAULT_ATTACK_RANGE),
      attack_cooldown(ATTACK_COOLDOWN), current_cooldown(0)
{
    spell = std::make_unique<DirectDamageSpell>();
}

EnemyTower::EnemyTower(const EnemyTower& other)
    : x(other.x), y(other.y), attack_range(other.attack_range),
      attack_cooldown(other.attack_cooldown), current_cooldown(other.current_cooldown)
{
    spell = std::make_unique<DirectDamageSpell>();
}

int EnemyTower::get_x() const { return x; }
int EnemyTower::get_y() const { return y; }
int EnemyTower::get_attack_range() const { return attack_range; }
int EnemyTower::get_cooldown() const { return current_cooldown; }

bool EnemyTower::is_player_in_range(const Player* player) const
{
    if (!player) return false;
    
    int distance = std::abs(player->get_x() - x) + std::abs(player->get_y() - y);
    return distance <= attack_range;
}

void EnemyTower::attack_player(Player* player, GameMap& map)
{
    if (!player || !player->is_alive() || !spell) return;
    
    spell->cast(map, player->get_x(), player->get_y());
    current_cooldown = attack_cooldown;
}

void EnemyTower::update()
{
    if (current_cooldown > 0)
    {
        current_cooldown--;
    }
}

bool EnemyTower::can_attack() const
{
    return current_cooldown == 0;
}

void EnemyTower::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&current_cooldown), sizeof(current_cooldown));
}

void EnemyTower::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&current_cooldown), sizeof(current_cooldown));
}