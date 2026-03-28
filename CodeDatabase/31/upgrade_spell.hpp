#ifndef UPGRADE_SPELL_HPP
#define UPGRADE_SPELL_HPP

#include "spell.hpp"

class UpgradeSpell : public Spell {
    int active_upgrades;
public:
    UpgradeSpell() : active_upgrades(0) {}
    UpgradeSpell(nlohmann::json &j);

    bool needs_point() const;
    const char* get_abbrev() const;
    const char* get_color() const;
    int get_area_size() const;
    bool is_upgrade() const;
    int get_radius() const;
    void show_info() const;
    void upgrade();
    void level_up();
    bool use(Point point, Point player_pos);
    void set_active_upgrades(int count);
    int get_cost_step() const;
    bool can_apply_upgrades() const;
    std::vector<Entity*> get_pending_allies();
    nlohmann::json save();
};

#endif