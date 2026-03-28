#include "direct_damage_spell.hpp"
#include "entities/entity.hpp"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(Field &field) : field(field) {}

void DirectDamageSpell::show_info() const {
    const char* color = get_color();
    std::cout << color << "╔════════════════════════╗" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << "   direct damage spell  " << color << "║" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " damage = " << damage << "  radius = " << radius << color << " ║" << "\033[0m" << std::endl;
    std::cout << color << "╚════════════════════════╝" << "\033[0m" << std::endl;
}

void DirectDamageSpell::upgrade() {
    radius += 2;
}

bool DirectDamageSpell::use(Point target, Point player_pos) {
    if (!target.inside_field(field.get_width(), field.get_height())) {
        return false;
    }
    if (std::abs(target.x - player_pos.x) > radius || std::abs(target.y - player_pos.y) > radius) {
        return false;
    }
    Cell &cell = field[target];
    if (cell.has_entity() && cell.get_entity().get_team() == Team::Enemy) {
        cell.get_entity().take_damage(damage);
        return true;
    }
    return false;
}

bool DirectDamageSpell::needs_point() const {
    return true;
}

const char* DirectDamageSpell::get_abbrev() const {
    return "dds";
}

const char* DirectDamageSpell::get_color() const {
    return "\033[38;2;26;232;36m";
}

int DirectDamageSpell::get_area_size() const {
    return 1;
}

bool DirectDamageSpell::is_upgrade() const {
    return false;
}

int DirectDamageSpell::get_radius() const {
    return radius;
}

void DirectDamageSpell::level_up() {
    damage += 3;
}

void DirectDamageSpell::set_active_upgrades(int count) {
    
}

int DirectDamageSpell::get_cost_step() const {
    return 3;
}

bool DirectDamageSpell::can_apply_upgrades() const {
    return true;
}

std::vector<Entity*> DirectDamageSpell::get_pending_allies() {
    return {};
}

nlohmann::json DirectDamageSpell::save()
{
    nlohmann::json j;
    j["radius"] = radius;
    j["damage"] = damage;
    j["type"] = "dds";
    return j;
}

DirectDamageSpell::DirectDamageSpell(nlohmann::json &j, Field &field)
    : radius(j["radius"]),
      damage(j["damage"]),
      field(field)
{}
