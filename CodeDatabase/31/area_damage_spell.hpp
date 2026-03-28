
#ifndef AREA_DAMAGE_SPELL_HPP
#define AREA_DAMAGE_SPELL_HPP

#include "spell.hpp"
#include "field/field.hpp"

class AreaDamageSpell : public Spell {
    int radius {3};
    int damage {3};
    int area_size {2};
    Field &field;
public:
    AreaDamageSpell(Field &field);
    AreaDamageSpell(nlohmann::json &j, Field &field);
    void upgrade();
    void level_up();
    bool use(Point target, Point player_pos);
    bool needs_point() const;
    const char* get_abbrev() const;
    const char* get_color() const;
    int get_area_size() const;
    bool is_upgrade() const;
    int get_radius() const;
    void show_info() const;
    int get_cost_step() const;
    void set_active_upgrades(int count);
    bool can_apply_upgrades() const;
    std::vector<Entity*> get_pending_allies();
    nlohmann::json save();
};

#endif