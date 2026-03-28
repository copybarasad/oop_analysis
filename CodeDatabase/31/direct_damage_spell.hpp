#ifndef DIRECT_DAMAGE_SPELL_HPP
#define DIRECT_DAMAGE_SPELL_HPP

#include "spell.hpp"
#include "field/field.hpp"

class DirectDamageSpell : public Spell {
    int radius {3};
    int damage {3};
    Field &field;
public:
    DirectDamageSpell(Field &field);
    DirectDamageSpell(nlohmann::json &j, Field &field);
    void show_info() const;
    void upgrade();
    void level_up();
    bool use(Point target, Point player_pos);
    bool needs_point() const;
    const char* get_abbrev() const;
    const char* get_color() const;
    int get_area_size() const;
    bool is_upgrade() const;
    int get_radius() const;
    int get_cost_step() const;
    void set_active_upgrades(int count);
    bool can_apply_upgrades() const;
    std::vector<Entity*> get_pending_allies();
    nlohmann::json save();
};

#endif