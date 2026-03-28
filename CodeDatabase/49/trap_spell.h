#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include <cmath>
#include "trap_spell_interface.h"

class TrapSpell : public ITrapSpell
{
private:
    std::string name;
    int mana_cost;
    int cooldown;
    int max_cooldown;
    int damage;
    int cast_range;

    static constexpr int COST = 10;
    static constexpr int DEFAULT_COOLDOWN = 5;
    static constexpr int DAMAGE = 2;
    static constexpr int CAST_RANGE = 5;

public:
    TrapSpell();
    
    int get_mana_cost() const override;
    std::string get_name() const override;
    int get_cooldown() const override;
    int get_max_cooldown() const override;
    int get_damage() const override;
    int get_cast_range() const override;

    void set_cooldown(int value) override;

    bool is_available(int player_mana) const override;
    void update_cooldown() override;
    
    void cast(GameMap& map, int target_x, int target_y) override;

    void save_binary(std::ostream& os) const override;
    void load_binary(std::istream& is) override;
};

#endif