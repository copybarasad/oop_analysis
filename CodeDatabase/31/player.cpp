#include "player.hpp"
#include "entities/human.hpp"
#include "entity.hpp"
#include "field/cell.hpp"
#include "field/field.hpp"
#include <stdexcept>
#include <vector>
#include <nlohmann/json.hpp>
#include <format>

Player::Player(Field &field, Point position, int health)
    : Human(field, position, health), attack_type(AttackType::Melee) {
}

void Player::add_score(int points) {
    score += points;
}

int Player::get_score() const {
    return score;
}

void Player::switch_attack_type() {
    attack_type = (AttackType)(((int)attack_type + 1) % (int)AttackType::TotalCount);
}

AttackType Player::get_attack_type() const {
    return attack_type;
}

Entity& Player::attack(Point point) {
    int radius = (attack_type == AttackType::Melee) ? melee_radius : ranged_radius;
    if (std::abs(point.x - position.x) > radius || std::abs(point.y - position.y) > radius || point == position) return *this;
    if (!point.inside_field(field.get_width(), field.get_height())) return *this;
    if (!field[point].has_entity()) return *this;
    Entity &entity = field[point].get_entity();
    if (entity.get_team() != Team::Enemy) return *this;
    
    int damage = (attack_type == AttackType::Melee) ? melee_damage : ranged_damage;
    entity.take_damage(damage);
    add_score(damage);
    return entity;
}

std::string Player::make_step() {
    if (field[position].get_type() == CellType::Slow && count_skips == 0) action = Action::skip_step;
    if (count_skips == 1) count_skips = 0;
    if (!is_skip_step) {
        switch (action) {
            case Action::left: {
                Point from = get_position();
                if (left_motion()) {
                    Point to = get_position();
                    return std::format("Player moved from {} to {}\n", from.str(), to.str());
                }
                break;
            }
            case Action::right: {
                Point from = get_position();
                if (right_motion()) {
                    Point to = get_position();
                    return std::format("Player moved from {} to {}\n", from.str(), to.str());
                }
                break;
            }
            case Action::up: {
                Point from = get_position();
                if (up_motion()) {
                    Point to = get_position();
                    return std::format("Player moved from {} to {}\n", from.str(), to.str());
                }
                break;
            }
            case Action::down: {
                Point from = get_position();
                if (down_motion()) {
                    Point to = get_position();
                    return std::format("Player moved from {} to {}\n", from.str(), to.str());
                }
                break;
            }
            case Action::attack: {
            Entity& entity = attack(action_point);
            EntityType type = entity.get_entity_type();
            if (type == EntityType::Enemy || type == EntityType::EnemyBuilding || 
                type == EntityType::EnemyTower) {
                    return std::format("PLayer attacked {} at {} and dealt damage {}\n", 
                        entity.str(), action_point.str(), get_current_damage());
                }
                break;
            }
            case Action::switch_attack_type: {
                switch_attack_type();
                AttackType to = attack_type;
                if (to == AttackType::Melee) {
                    return "Player swithed attack type from to Melee\n";
                }
                else {
                    return "Player swithed attack type from to Ranged\n";
                }
                break;
            }
            case Action::skip_step: {
                count_skips++;
                return "Player skiped step\n";
                break;
            }
            default:
                throw std::invalid_argument("incorect player action\n");
                return "incorrect action\n";
        }
    }
    return "";
}

Point Player::rand_enemy() {
    int radius = (attack_type == AttackType::Melee) ? melee_radius : ranged_radius;
    std::vector<Point> nearest, building, others;
    
    for (int x = std::max(0, position.x - radius); x <= std::min(field.get_width() - 1, position.x + radius); x++) {
        for (int y = std::max(0, position.y - radius); y <= std::min(field.get_height() - 1, position.y + radius); y++) {
            if (std::abs(x - position.x) > radius || std::abs(y - position.y) > radius) continue;
            if (x == position.x && y == position.y) continue;
            
            Cell &cell = field[{x, y}];
            if (cell.has_entity() && cell.get_entity().get_team() == Team::Enemy) {
                Point p = {x, y};
                if ((x == position.x && std::abs(y - position.y) == 1) || 
                    (y == position.y && std::abs(x - position.x) == 1)) {
                    nearest.push_back(p);
                } else if (cell.get_entity().get_entity_type() == EntityType::EnemyBuilding) {
                    building.push_back(p);
                } else {
                    others.push_back(p);
                }
            }
        }
    }
    
    if (!nearest.empty()) return nearest[rand() % nearest.size()];
    if (!building.empty()) return building[rand() % building.size()];
    if (!others.empty()) return others[rand() % others.size()];
    
    std::vector<Point> neighbors = position.get_neighbors(field.get_width(), field.get_height());
    return neighbors.empty() ? position : neighbors[rand() % neighbors.size()];
}

void Player::set_action(Action action) { 
    this->action = action; 
}

void Player::set_point(Point point) {
    this->action_point = point;
}

Team Player::get_team() const {
    return Team::Player;
}

EntityType Player::get_entity_type() const {
    return EntityType::Player;
}

void Player::decrease_score(int value) {
    score -= value;
}

void Player::upgrade_damage() {
    melee_damage++;
    ranged_damage++;
}

int Player::get_melee_damage() const {
    return melee_damage;
}

int Player::get_ranged_damage() const {
    return ranged_damage;
}

int Player::get_ranged_radius() const {
    return ranged_radius;
}

nlohmann::json Player::save() const {
    nlohmann::json j;
    j["x"] = position.x;
    j["y"] = position.y;
    j["health"] = health;
    j["score"] = score;
    j["attack_type"] = (int)attack_type;
    j["melee_damage"] = melee_damage;
    j["ranged_damage"] = ranged_damage;
    return j;
}

void Player::set_position(Point pos) {
    Entity::set_position(pos);
}