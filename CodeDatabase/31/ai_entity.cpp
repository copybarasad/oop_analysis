#include "ai_entity.hpp"
#include "field/field.hpp"
#include <cstdlib>
#include <format>

AIEntity::AIEntity(Field &field, Point position, int health)
    : Human(field, position, health) {}

Entity* AIEntity::find_closest_target(Team target_team, int scan_radius) {
    Entity *target = nullptr;
    int target_dist = INT_MAX;

    for (int dx = -scan_radius; dx <= scan_radius; dx++) {
        for (int dy = -scan_radius; dy <= scan_radius; dy++) {
            int x = position.x + dx;
            int y = position.y + dy;
            
            if (x < 0 || x >= field.get_width() || 
                y < 0 || y >= field.get_height()) {
                continue;
            }

            Cell &cell = field.get_cell({x, y});
            if (!cell.has_entity()) {
                continue;
            }

            Entity &entity = cell.get_entity();
            if (entity.get_team() != target_team) {
                continue;
            }

            int dist = (position - Point{x, y}).len2();
            if (dist < target_dist) {
                target_dist = dist;
                target = &entity;
            }
        }
    }

    return target;
}

Entity* AIEntity::find_visible_target(Team target_team, int vis_radius) {
    return find_closest_target(target_team, vis_radius);
}

Human::Direction AIEntity::get_best_dir(Point to) {
    Human::Direction dir = Direction::TotalCount;
    int dir_cost = (to - get_position()).len2();

    for (int i = 0; i < (int)Direction::TotalCount; i++) {
        Point offset = Human::get_direction_offset((Human::Direction)i);
        Point p = get_position() + offset;
        
        if (p.x < 0 || p.x >= field.get_width() || 
            p.y < 0 || p.y >= field.get_height() ||
            field.get_cell(p).get_type() == CellType::Impassible) {
            continue;
        }

        int dist = (p - to).len2();
        if (dist < dir_cost) {
            dir = (Human::Direction)i;
            dir_cost = dist;
        }
    }

    return dir;
}

std::string AIEntity::idle_step() {
    Direction dir = (Direction)(rand() % (int)Direction::TotalCount);
    Point from = position;
    if (move_to(get_direction_offset(dir))) {
        Point to = position;
        return std::format("Allay moved from {} to {}\n", from.str(), to.str());
    }
    return "";
}