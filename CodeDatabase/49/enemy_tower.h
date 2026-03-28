#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "../player/player.h"
#include "../spells/direct_damage_spell.h"

class EnemyTower
{
private:
    int x, y;
    int attack_range;
    int attack_cooldown;
    int current_cooldown;
    std::unique_ptr<ISpell> spell;
    
    static constexpr int DEFAULT_ATTACK_RANGE = 3;
    static constexpr int ATTACK_COOLDOWN = 5;
    static constexpr int TOWER_DAMAGE = 2;
public:
    EnemyTower(int start_x, int start_y);
    EnemyTower(const EnemyTower& other);
    
    int get_x() const;
    int get_y() const;
    int get_attack_range() const;
    int get_cooldown() const;
    
    bool is_player_in_range(const Player* player) const;
    void attack_player(Player* player, GameMap& map);

    void update();
    bool can_attack() const;

    void save_binary(std::ostream& os) const;
    void load_binary(std::istream& is);
};

#endif