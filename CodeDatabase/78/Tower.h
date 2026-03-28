#ifndef TOWER_H
#define TOWER_H

#include "Entity.h"
#include "DirectDamageSpell.h"

class GameField;
class Player;

class Tower: public Entity{
public:
    Tower(int health, int x, int y,
            int range_spawn = 2,
            int max_cooldown_spawn = 10,
            int max_cooldown_attack = 5,
            int health_spawned_enemy = 50,
            float damage_multiplier = 0.2f,
            WeaponType weapon = WeaponType::UNARMED
        ) : Entity(health, x, y, weapon),
            range_spawn_(range_spawn),
            cooldown_spawn_(0),
            cooldown_attack_(0),
            max_cooldown_spawn_(max_cooldown_spawn),
            max_cooldown_attack_(max_cooldown_attack),
            health_spawned_enemy_(health_spawned_enemy),
            damage_multiplier_(damage_multiplier),
            spell_() {}
    
    ~Tower() = default;

    void reduce_cooldown_spawn() { if(cooldown_spawn_ > 0) cooldown_spawn_--; }
    void reduce_cooldown_attack() { if(cooldown_attack_ > 0) cooldown_attack_--; }

    bool can_spawn() const { return cooldown_spawn_ <= 0; }
    bool can_attack() const { return cooldown_attack_ <= 0; }

    void set_cooldown_spawn(int cooldown_spawn) { cooldown_spawn_ = cooldown_spawn; }
    void set_cooldown_attack(int cooldown_attack) { cooldown_attack_ = cooldown_attack; }

    void reset_cooldown_spawn() { cooldown_spawn_ = max_cooldown_spawn_; }
    void reset_cooldown_attack() { cooldown_attack_ = max_cooldown_attack_; }

    int get_range_spawn() const { return range_spawn_; }
    int get_range_attack() const { return spell_.get_range()/2 + 1; }
    
    void update(GameField& game_field);
    bool spawn_enemy(GameField& game_field);
    bool attack(GameField& game_field);
    void show_stats() const override;
    std::string serialize() const;

private:
    int range_spawn_;
    int cooldown_spawn_;
    int max_cooldown_spawn_;

    int cooldown_attack_;
    int max_cooldown_attack_;

    int health_spawned_enemy_;

    DirectDamageSpell spell_;
    float damage_multiplier_;
};

#endif