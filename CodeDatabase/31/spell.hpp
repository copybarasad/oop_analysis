#ifndef SPELL_HPP
#define SPELL_HPP

#include "field/point.hpp"
#include <vector>
#include <nlohmann/json.hpp>

class Entity;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool use(Point target, Point player_pos) = 0;
    virtual void upgrade() = 0;
    virtual void level_up() = 0;
    virtual bool needs_point() const = 0;
    virtual void show_info() const = 0;
    virtual const char* get_abbrev() const = 0;
    virtual const char* get_color() const = 0;
    virtual int get_area_size() const = 0;
    virtual bool is_upgrade() const = 0;
    virtual int get_radius() const = 0;
    virtual void set_active_upgrades(int count) = 0;
    virtual int get_cost_step() const = 0;
    virtual bool can_apply_upgrades() const = 0;
    virtual std::vector<Entity*> get_pending_allies() = 0;
    virtual nlohmann::json save() = 0;
};

#endif