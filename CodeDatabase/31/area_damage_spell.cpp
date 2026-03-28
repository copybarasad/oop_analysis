#include "area_damage_spell.hpp"
#include "entities/entity.hpp"
#include <iostream>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(Field &field) : field(field) {}

void AreaDamageSpell::upgrade() {
    area_size++;
}

bool AreaDamageSpell::use(Point target, Point player_pos) {
    if (!target.inside_field(field.get_width(), field.get_height())) {
        return false;
    }
    if (std::abs(target.x - player_pos.x) > radius || std::abs(target.y - player_pos.y) > radius) {
        return false;
    }
    for (int x = target.x; x < std::min(field.get_width(), target.x + area_size); x++) {
        for (int y = target.y; y < std::min(field.get_height(), target.y + area_size); y++) {
            Cell &cell = field[{x, y}];
            if (cell.has_entity() && cell.get_entity().get_team() == Team::Enemy) {
                cell.get_entity().take_damage(damage);
            }
        }
    }
    return true;
}

bool AreaDamageSpell::needs_point() const {
    return true;
}

const char* AreaDamageSpell::get_abbrev() const {
    return "ads";
}

const char* AreaDamageSpell::get_color() const {
    return "\033[38;5;190m";
}

int AreaDamageSpell::get_area_size() const {
    return area_size;
}

bool AreaDamageSpell::is_upgrade() const {
    return false;
}

int AreaDamageSpell::get_radius() const {
    return radius;
}

void AreaDamageSpell::show_info() const {
    const char* color = get_color();
    std::cout << color << "╔════════════════════════════════════╗" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << "          area damage spell         " << color << "║" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " damage = " << damage << "  area = " << area_size << "x" << area_size << "  radius = " << radius << color << " ║" << "\033[0m" << std::endl;
    std::cout << color << "╚════════════════════════════════════╝" << "\033[0m" << std::endl;
}

void AreaDamageSpell::level_up() {
    damage += 2;
}

void AreaDamageSpell::set_active_upgrades(int count) {
    
}

int AreaDamageSpell::get_cost_step() const {
    return 5;
}

bool AreaDamageSpell::can_apply_upgrades() const {
    return true;
}

std::vector<Entity*> AreaDamageSpell::get_pending_allies() {
    return {};
}

nlohmann::json AreaDamageSpell::save()
{
    nlohmann::json j;
    
    j["radius"] = radius;
    j["damage"] = damage;
    j["area_size"] = area_size;
    j["type"] = "ads";

    return j;
}

AreaDamageSpell::AreaDamageSpell(nlohmann::json &j, Field &field)
    : radius(j["radius"]),
      damage(j["damage"]),
      area_size(j["area_size"]),
      field(field)
{}
    