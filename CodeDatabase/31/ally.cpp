#include "ally.hpp"
#include <cmath>
#include <iostream>
#include "field/field.hpp"
#include <format>
#include <set>

Ally::Ally(Field &field, Point position, int health)
    : AIEntity(field, position, health) {}

Team Ally::get_team() const {
    return Team::Player;
}

EntityType Ally::get_entity_type() const {
    return EntityType::Ally;
}

std::string Ally::make_step() {
    std::set<Point> visible_set;
    
    int vis_radius = 10;
    float a_step = 1.f / vis_radius;

    for (float a = 0; a < 2*M_PI; a += a_step) {
        float dirx = cos(a), diry = sin(a);
        float curx = position.x, cury = position.y;

        for (int i = 0; i < vis_radius; i++) {
            Point p = Point { (int)curx, (int)cury };
            if (!p.is_in_rect({ 0, 0 }, { field.get_width(), field.get_height() }) || field[{p.x, p.y}].get_type() == CellType::Impassible) {
                break;
            }

            visible_set.insert(p);
            curx += dirx;
            cury += diry;
        }
    }

    Entity *target = nullptr;
    int target_dist = INT_MAX;

    for (Point p : visible_set) {
        Cell &cell = field[{p.x, p.y}];
        if (!cell.has_entity()) {
            continue;
        }

        Entity &entity = cell.get_entity();
        Team team = entity.get_team();
        if (team == this->get_team() || team == Team::Neutral) {
            continue;
        }

        int dist = (get_position() - entity.get_position()).len2();
        if (dist < target_dist) {
            target_dist = dist;
            target = &entity;
        }
    }

    if (target == nullptr) {
        return idle_step();
    }

    int radius = (attack_type == AllyAttackType::Melee) ? melee_radius : ranged_radius;
    int damage = (attack_type == AllyAttackType::Melee) ? melee_damage : ranged_damage;
    if (target_dist <= radius*radius) {
        target->take_damage(damage);
        return std::format("Ally dealt damage {} to {} at {}\n", damage, target->str(), target->get_position().str());
    }
    

    Direction dir = get_best_dir(target->get_position());
    if (dir == Direction::TotalCount) {
        return idle_step();
    }
    else {
        Point from = position;
        if(move_to(get_direction_offset(dir))) {
            Point to = position;
            return std::format("Ally moved from {} to {}\n", from.str(), to.str());
        }

    }
    return "";
}

void Ally::switch_attack_type() {
    attack_type = (attack_type == AllyAttackType::Melee) ? AllyAttackType::Ranged : AllyAttackType::Melee;
}