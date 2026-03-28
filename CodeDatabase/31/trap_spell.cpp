#include "trap_spell.hpp"
#include "field/cell.hpp"
#include <iostream>
#include <cmath>

TrapSpell::TrapSpell(Field &field) : field(field) {}

bool TrapSpell::use(Point position, Point player_pos) {
    if (!position.inside_field(field.get_width(), field.get_height())) {
        return false;
    }
    Cell &cell = field[position];
    if (cell.get_type() == CellType::Impassible || cell.has_entity()) {
        return false;
    }
    cell.set_trap(damage);
    return true;
}

void TrapSpell::upgrade() {
    damage *= 2;
}

bool TrapSpell::needs_point() const {
    return true;
}

const char* TrapSpell::get_abbrev() const {
    return "ts";
}

const char* TrapSpell::get_color() const {
    return "\033[38;5;51m";
}

int TrapSpell::get_area_size() const {
    return 1;
}

bool TrapSpell::is_upgrade() const {
    return false;
}

int TrapSpell::get_radius() const {
    return 0;
}

void TrapSpell::show_info() const {
    const char* color = get_color();
    std::cout << color << "╔════════════════════╗" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " trap spell        " << color << " ║" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " damage = " << damage << "        " << color << " ║" << "\033[0m" << std::endl;
    std::cout << color << "╚════════════════════╝" << "\033[0m" << std::endl;
}

void TrapSpell::level_up() {
    damage += 2;
}

void TrapSpell::set_active_upgrades(int count) {
   
}

int TrapSpell::get_cost_step() const {
    return 4;
}

bool TrapSpell::can_apply_upgrades() const {
    return true;
}

std::vector<Entity*> TrapSpell::get_pending_allies() {
    return {};
}

nlohmann::json TrapSpell::save()
{
    nlohmann::json j;
    j["damage"] = damage;
    j["type"] = "ts";
    return j;
}

TrapSpell::TrapSpell(nlohmann::json &j, Field &field)
    : damage(j["damage"]),
      field(field)
{}