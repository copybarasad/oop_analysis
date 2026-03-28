#include "game/game.hpp"
#include "ally_call_spell.hpp"
#include <iostream>
#include "entities/ally.hpp"

AllyCallSpell::AllyCallSpell(Field &field) : field(field) {}

bool AllyCallSpell::needs_point() const {
    return false;
}

const char* AllyCallSpell::get_abbrev() const {
    return "acs";
}

const char* AllyCallSpell::get_color() const {
    return "\033[38;5;200m";
}

int AllyCallSpell::get_area_size() const {
    return 1;
}

bool AllyCallSpell::is_upgrade() const {
    return false;
}

int AllyCallSpell::get_radius() const {
    return 0;
}

void AllyCallSpell::show_info() const {
    const char* color = get_color();
    std::cout << color << "╔════════════════════════╗" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " ally call spell        " << color << "║" << "\033[0m" << std::endl;
    std::cout << color << "║" << "\033[0m" << " allies = " << allies_count << (allies_count < 10 ? "             " : "            ") << color << "║" << "\033[0m" << std::endl;
    std::cout << color << "╚════════════════════════╝" << "\033[0m" << std::endl;
}

void AllyCallSpell::upgrade() {
    allies_count *= 2;
}

void AllyCallSpell::create_ally(Point player_pos) {
    int radius = 1;
    std::vector<Point> candidates;
    while (candidates.empty()) {
        for (int x = std::max(0, player_pos.x - radius); x <= std::min(field.get_width() - 1, player_pos.x + radius); x++) {
            for (int y = std::max(0, player_pos.y - radius); y <= std::min(field.get_height() - 1, player_pos.y + radius); y++) {
                if (x == player_pos.x && y == player_pos.y) continue;
                Cell &cell = field[{x, y}];
                if (cell.get_type() != CellType::Impassible && !cell.has_entity()) {
                    candidates.push_back({x, y});
                }
            }
        }
        radius++;
    }
    Point ally_pos = candidates[rand() % candidates.size()];
    Ally *ally = new Ally(field, ally_pos);
    pending_allies.push_back(ally);
}

bool AllyCallSpell::use(Point point, Point player_pos) {
    pending_allies.clear();
    for (int i = 0; i < allies_count; i++) {
        create_ally(player_pos);
    }
    return true;
}

void AllyCallSpell::level_up()
{
    allies_count++;
}

std::vector<Entity*> AllyCallSpell::get_pending_allies() {
    return pending_allies;
}

int AllyCallSpell::get_cost_step() const {
    return 0;
}

void AllyCallSpell::set_active_upgrades(int count) {}

bool AllyCallSpell::can_apply_upgrades() const {
    return true;
}

nlohmann::json AllyCallSpell::save()
{
    nlohmann::json j;
    j["allies_count"] = allies_count;
    j["type"] = "acs";
    return j;
}

AllyCallSpell::AllyCallSpell(nlohmann::json &j, Field &field)
    : allies_count(j["allies_count"]), field(field) {}