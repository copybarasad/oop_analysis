#ifndef HEALTH_SPELL_HPP
#define HEALTH_SPELL_HPP

#include "spell.hpp"

class Player;

class HealthSpell : public Spell {
    int health_amount;
    int base_health_amount;
    Player &player;
public:
    HealthSpell(Player &player, int health_amount = 10);
    HealthSpell(nlohmann::json &j, Player &player);
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
    int get_cost_step() const;
    bool can_apply_upgrades() const;
    void set_active_upgrades(int count);
    std::vector<Entity*> get_pending_allies();
    nlohmann::json save();
};

#endif