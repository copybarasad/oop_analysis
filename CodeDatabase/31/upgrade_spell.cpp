#include "upgrade_spell.hpp"
#include <iostream>

bool UpgradeSpell::needs_point() const {
    return false;
}

const char* UpgradeSpell::get_abbrev() const {
    return "us";
}

const char* UpgradeSpell::get_color() const {
    return "\033[95m";
}

int UpgradeSpell::get_area_size() const {
    return 1;
}

bool UpgradeSpell::is_upgrade() const {
    return true;
}

int UpgradeSpell::get_radius() const {
    return 0;
}

void UpgradeSpell::show_info() const {
    const char* color = get_color();
    std::cout << color << "╔════════════════════╗" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " upgrade spell      " << color << "║" << "\033[0m" << std::endl;
    std::cout << color << "╚════════════════════╝" << "\033[0m" << std::endl;
}

void UpgradeSpell::upgrade() {}

bool UpgradeSpell::use(Point point, Point player_pos) {
    return false;
}

void UpgradeSpell::level_up() {}

void UpgradeSpell::set_active_upgrades(int count) {

}

int UpgradeSpell::get_cost_step() const {
    return 10;
}

bool UpgradeSpell::can_apply_upgrades() const {
    return false;
}

std::vector<Entity*> UpgradeSpell::get_pending_allies() {
    return {};
}

nlohmann::json UpgradeSpell::save()
{
    nlohmann::json j;
    j["active_upgrades"] = active_upgrades;
    j["type"] = "us";
    return j;
}

UpgradeSpell::UpgradeSpell(nlohmann::json &j)
    : active_upgrades(j["active_upgrades"])
{}