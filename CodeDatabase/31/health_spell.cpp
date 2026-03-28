#include "health_spell.hpp"
#include "entities/player.hpp"
#include <iostream>

HealthSpell::HealthSpell(Player &player, int health_amount) : player(player), health_amount(health_amount), base_health_amount(health_amount) {}

bool HealthSpell::needs_point() const {
    return false;
}

const char* HealthSpell::get_abbrev() const {
    return "hs";
}

const char* HealthSpell::get_color() const {
    return "\033[38;5;46m";
}

int HealthSpell::get_area_size() const {
    return 1;
}

bool HealthSpell::is_upgrade() const {
    return false;
}

int HealthSpell::get_radius() const {
    return 0;
}

void HealthSpell::show_info() const {
    const char* color = get_color();
    std::cout << color << "╔════════════════════════╗" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " health spell           " << color << "║" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " health = " << health_amount << (health_amount < 10 ? "             " : "            ") << color << "║" << "\033[0m" << std::endl;
    std::cout << color << "╚════════════════════════╝" << "\033[0m" << std::endl;
}

void HealthSpell::upgrade() {
    health_amount *= 2;
}

void HealthSpell::level_up() {
    health_amount += 5;
    base_health_amount = health_amount;
}

bool HealthSpell::use(Point point, Point player_pos) {
    player.restore_health(health_amount);
    return true;
}

int HealthSpell::get_cost_step() const {
    return 0;
}

bool HealthSpell::can_apply_upgrades() const {
    return false;
}

void HealthSpell::set_active_upgrades(int count) {}

std::vector<Entity*> HealthSpell::get_pending_allies() {
    return {};
}

nlohmann::json HealthSpell::save()
{
    nlohmann::json j;
    j["health_amount"] = health_amount;
    j["base_health_amount"] = base_health_amount;
    j["type"] = "hs";
    return j;
}

HealthSpell::HealthSpell(nlohmann::json &j, Player &player)
    : health_amount(j["health_amount"]),
      base_health_amount(j["base_health_amount"]),
      player(player)
{}