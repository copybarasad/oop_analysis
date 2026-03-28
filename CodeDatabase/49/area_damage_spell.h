#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include <cmath>
#include <vector>
#include "area_damage_spell_interface.h"

class AreaDamageSpell : public IAreaDamageSpell
{
private:
    std::string name;
    int mana_cost;
    int cooldown;
    int max_cooldown;
    int damage;
    int cast_range;
    int area_size;

    static constexpr int SCORE_PER_DESTROY_BUILDING = 50;
    static constexpr int COST = 50;
    static constexpr int DEFAULT_COOLDOWN = 10;
    static constexpr int DAMAGE = 4;
    static constexpr int CAST_RANGE = 5;
    static constexpr int AREA_SIZE = 2;

public:
    AreaDamageSpell();
    
    int get_mana_cost() const override;
    std::string get_name() const override;
    int get_cooldown() const override;
    int get_max_cooldown() const override;
    int get_damage() const override;
    int get_cast_range() const override;
    int get_area_size() const override;

    void set_cooldown(int value) override;

    bool is_available(int player_mana) const override;
    void update_cooldown() override;
    
    void cast(GameMap& map, int target_x, int target_y) override;

    void save_binary(std::ostream& os) const override;
    void load_binary(std::istream& is) override;
};

#endif