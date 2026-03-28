#ifndef ALLY_CALL_SPELL_HPP
#define ALLY_CALL_SPELL_HPP

#include "field/field.hpp"
#include "spell.hpp"

class Entity;
class AllyCallSpell : public Spell {
    Field &field;
    int allies_count {1};
    std::vector<Entity*> pending_allies;
public:
    AllyCallSpell(Field &field);
    AllyCallSpell(nlohmann::json &j, Field &field);
    bool use(Point point, Point player_pos);
    void create_ally(Point player_pos);
    bool needs_point() const;
    const char* get_abbrev() const;
    const char* get_color() const;
    int get_area_size() const;
    bool is_upgrade() const;
    int get_radius() const;
    void show_info() const;
    void upgrade();
    void level_up();
    std::vector<Entity*> get_pending_allies();
    int get_cost_step() const;
    void set_active_upgrades(int count);
    bool can_apply_upgrades() const;
    nlohmann::json save();
};

#endif