#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include <cmath>
#include "direct_damage_spell_interface.h"

class DirectDamageSpell : public IDirectDamageSpell
{
private:
    std::string name;
    int mana_cost;
    int cooldown;
    int max_cooldown;
    int damage;
    int cast_range;

    static constexpr int SCORE_PER_DESTROY_BUILDING = 50;
    static constexpr int COST = 30;
    static constexpr int DEFAULT_COOLDOWN = 8;
    static constexpr int DAMAGE = 6;
    static constexpr int TOWER_DAMAGE = 2;
    static constexpr int CAST_RANGE = 5;

public:
    DirectDamageSpell();
    
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