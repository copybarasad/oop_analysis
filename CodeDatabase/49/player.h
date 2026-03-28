#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "combat_mode.h"
#include "../spells/spell_hand.h"

class GameMap;
class ISpell;

class Player
{
private:
    int x, y;
    int score;
    int mana;
    int max_mana;
    int health;
    int max_health;
    int melee_damage;
    int range_damage;
    CombatMode combat_mode;
    int slowdown_interval;
    int slowdown_cooldown;
    SpellHand spell_hand;

    static constexpr int START_MANA = 25;
    static constexpr int MAX_MANA = 100;
    static constexpr int INCREASE_MANA = 25;
    static constexpr int DEFAULT_HEALTH = 5;
    static constexpr int INCREASE_HEALTH = 1;
    static constexpr int MELEE_DAMAGE = 3;
    static constexpr int RANGE_DAMAGE = 2;
    static constexpr int SLOWDOWN_INTERVAL = 2;
    static constexpr int RANGED_ATTACK_RANGE = 3;
    static constexpr int MANA_PER_KILL_MELEE = 50;
    static constexpr int MANA_PER_KILL_RANGE = 25;
    static constexpr int SPELL_DROP_CHANCE = 50;

public:
    static constexpr int SCORE_PER_KILL = 15;
    
    Player(int start_x, int start_y);
    Player(const Player& other);
    Player& operator=(const Player& other);

    int get_x() const;
    int get_y() const;
    int get_score() const;
    int get_mana() const;
    int get_max_mana() const;
    int get_health() const;
    int get_max_health() const;
    int get_damage() const;
    CombatMode get_combat_mode() const;
    SpellHand& get_spell_hand();
    const SpellHand& get_spell_hand() const;

    void set_mana(int new_mana);
    void set_health(int new_health);
    void set_position(int new_x, int new_y);

    void add_score(int amount);
    void add_mana(int amount);
    void subtract_mana(int amount);
    bool is_alive() const;
    void take_damage(int enemy_damage);
    void switch_combat_mode();

    void upgrade_max_health();
    void upgrade_max_mana();
    
    void update();
    void reset_cooldown();
    bool can_move() const;

    bool move(GameMap& map, int new_x, int new_y);
    void attack(GameMap& map);

    void restore_health();

    void add_random_spell_to_hand();
    void cast_spell(ISpell* spell, GameMap& map, int target_x, int target_y);
    void update_spell_cooldown(ISpell* spell);

    void save_binary(std::ostream& os) const;
    void load_binary(std::istream& is);
};

#endif