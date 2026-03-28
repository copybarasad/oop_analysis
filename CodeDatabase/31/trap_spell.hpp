#ifndef TRAP_SPELL_HPP
#define TRAP_SPELL_HPP

#include "spell.hpp"
#include "field/field.hpp"

class TrapSpell : public Spell {
    int damage {3};
    Field &field;
public:
    TrapSpell(Field &field);
    TrapSpell(nlohmann::json &j, Field &field);    
    bool use(Point position, Point player_pos);
    void upgrade();
    void level_up();
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