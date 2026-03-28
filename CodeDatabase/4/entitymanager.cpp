#include "entitymanager.hpp"
#include "attacktower.hpp"
#include "field.hpp"
#include <cmath>
#include <algorithm>

EntityManager::EntityManager(int count_enemy, int temp_level, int width_param, int height_param)
    : level(temp_level) {
    int num_attack = 1 + (level >= 3 ? 1 : 0) + (level >= 5 ? 1 : 0);
    for (int i = 0; i < count_enemy; i++) {
        if (i < num_attack){
            int tower_id = 5000 + i;
            create_attack_tower(tower_id, 0, 0);
        }
        else if (static_cast<int>(towers.size()) < level) {
            create_tower(6, level, i);
        } else {
            create_mob(level, i, 0, 0);
        }
    }
}

void EntityManager::create_attack_tower(int id, int x, int y) {
    attack_towers.emplace_back(AttackTower(id), std::make_pair(x, y));
}

void EntityManager::create_ally(int id, int x, int y) {
    allies.emplace_back(Ally(id), std::make_pair(x, y));
}

void EntityManager::create_mob(int level, int id, int x, int y) {
    enemies.emplace_back(Enemy(level, id), std::make_pair(x, y));
}

void EntityManager::create_tower(int cooldown, int level, int id) {
    int tower_id = 100 + id;
    towers.emplace_back(Tower(cooldown, level, tower_id), std::make_pair(0, 0));
}

bool EntityManager::canMoveTo(const std::vector<std::vector<FieldCell>>& box, int x, int y) const {
    if (x < 0 || x >= static_cast<int>(box[0].size()) ||
        y < 0 || y >= static_cast<int>(box.size())) {
        return false;
    }
    return box[y][x].get_entity() == ZERO && box[y][x].get_type() != HARD;
}

void EntityManager::move_attack_towers(Field& field, EntityManager& roster, Hero& player) {
    for (auto& tower_pair : attack_towers) {
        AttackTower& tower = tower_pair.first;
        int tx = tower_pair.second.first;
        int ty = tower_pair.second.second;
        if (!tower.alive()) continue;
        tower.tick();
        tower.try_attack(field, roster, player, tx, ty);
    }
}

void EntityManager::move_ally(std::vector<std::vector<FieldCell>>& box, int hero_x, int hero_y) {
    for (auto& ally_pair : allies) {
        Ally& ally = ally_pair.first;
        if (ally.skip_turn) {
            ally.skip_turn = false;
            continue;
        }
        if (!ally.alive()) continue;
        int& ax = ally_pair.second.first;
        int& ay = ally_pair.second.second;
        int target_x = -1, target_y = -1;
        int min_dist = 1000;
        for (auto& mob_pair : enemies) {
            if (!mob_pair.first.alive()) continue;
            int mx = mob_pair.second.first;
            int my = mob_pair.second.second;
            int dist = std::abs(ax - mx) + std::abs(ay - my);
            if (dist < min_dist) {
                min_dist = dist;
                target_x = mx;
                target_y = my;
            }
        }
        if (min_dist <= 1 && min_dist > 0) {
            for (auto& mob_pair : enemies) {
                if (mob_pair.second.first == target_x && mob_pair.second.second == target_y) {
                    mob_pair.first.get_damage(ally.damage);
                    break;
                }
            }
            continue;
        }
        if (target_x == -1) continue;
        int dx = 0, dy = 0;
        if (ax < target_x) dx = 1;
        else if (ax > target_x) dx = -1;
        if (ay < target_y) dy = 1;
        else if (ay > target_y) dy = -1;
        int new_x = ax + dx;
        int new_y = ay + dy;
        if (std::abs(ax - target_x) < std::abs(ay - target_y)) {
            new_x = ax;
        } else {
            new_y = ay;
        }
        if ((new_x != ax || new_y != ay) &&
            new_x >= 0 && new_x < static_cast<int>(box[0].size()) &&
            new_y >= 0 && new_y < static_cast<int>(box.size()) &&
            box[new_y][new_x].get_entity() == ZERO &&
            box[new_y][new_x].get_type() != HARD) {
            box[ay][ax] = FieldCell(box[ay][ax].get_type());
            ax = new_x;
            ay = new_y;
            box[ay][ax] = FieldCell(box[ay][ax].get_type(), ALLY, ally.id);
        }
    }
}

void EntityManager::move_mob(std::vector<std::vector<FieldCell>>& box, Hero& player, int hero_x, int hero_y) {
    for (auto& mob_pair : enemies) {
        Enemy& mob = mob_pair.first;
        if (mob.skip_turn) {
            mob.skip_turn = false;
            continue;
        }
        int& mob_x = mob_pair.second.first;
        int& mob_y = mob_pair.second.second;
        if (!mob.alive()){
            player.take_mana(10);
            continue;
        }
        if ((std::abs(mob_x - hero_x) == 1 && std::abs(mob_y - hero_y) == 0) ||
            (std::abs(mob_x - hero_x) == 0 && std::abs(mob_y - hero_y) == 1)) {
            player.get_damage(mob.damage);
            continue;
        }
        int dx = 0, dy = 0;
        if (mob_x < hero_x) dx = 1;
        else if (mob_x > hero_x) dx = -1;
        if (mob_y < hero_y) dy = 1;
        else if (mob_y > hero_y) dy = -1;
        int dist_x = std::abs(mob_x - hero_x);
        int dist_y = std::abs(mob_y - hero_y);
        int new_x = mob_x;
        int new_y = mob_y;
        if (dist_x > dist_y) {
            new_x = mob_x + dx;
        } else {
            new_y = mob_y + dy;
        }
        if (new_x == hero_x && new_y == hero_y) continue;
        if ((new_x != mob_x || new_y != mob_y) && canMoveTo(box, new_x, new_y)) {
            box[mob_y][mob_x] = FieldCell(box[mob_y][mob_x].get_type());
            mob_x = new_x;
            mob_y = new_y;
            box[mob_y][mob_x] = FieldCell(box[mob_y][mob_x].get_type(), MOB, mob.id);
            if (box[mob_y][mob_x].get_type() == TRAP) {
                mob.get_damage(100);
                box[mob_y][mob_x].set_type(EMPTY);
            }
            if (box[mob_y][mob_x].get_type() == SLOW) {
                mob.skip_turn = true;
            }
        }
    }
}

void EntityManager::move_tower(std::vector<std::vector<FieldCell>>& box) {
    for (auto& tower_pair : towers) {
        Tower& tower = tower_pair.first;
        int& tower_x = tower_pair.second.first;
        int& tower_y = tower_pair.second.second;
        if (!tower.alive()) continue;
        if (tower.current_timer > 0) {
            tower.current_timer--;
            continue;
        }
        std::vector<std::pair<int, int>> directions = {{1,0}, {-1,0}, {0,1}, {0,-1}};
        for (const auto& dir : directions) {
            int new_x = tower_x + dir.first;
            int new_y = tower_y + dir.second;
            if (canMoveTo(box, new_x, new_y)) {
                create_mob(level, enemy_id_counter, new_x, new_y);
                box[new_y][new_x] = FieldCell(box[new_y][new_x].get_type(), MOB, enemy_id_counter);
                enemy_id_counter++;
                tower.current_timer = tower.cooldown;
                break;
            }
        }
    }
}

void EntityManager::atack_enemy(int dmg, int id) {
    for (auto& mob_pair : enemies) {
        if (mob_pair.first.id == id) {
            mob_pair.first.get_damage(dmg);
            return;
        }
    }
    for (auto& tower_pair : towers) {
        if (tower_pair.first.id == id) {
            tower_pair.first.get_damage(dmg);
            return;
        }
    }
    for (auto& at_pair : attack_towers) {
        if (at_pair.first.id == id) {
            at_pair.first.get_damage(dmg);
            return;
        }
    }
}

void EntityManager::update() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const auto& e_pair) { return !(e_pair.first.alive()); }),
        enemies.end()
    );
    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [](const auto& t_pair) { return !(t_pair.first.alive()); }),
        towers.end()
    );
    allies.erase(
        std::remove_if(allies.begin(), allies.end(),
            [](const auto& a_pair) { return !(a_pair.first.alive()); }),
        allies.end()
    );
    attack_towers.erase(
        std::remove_if(attack_towers.begin(), attack_towers.end(),
            [](const auto& f_pair) { return !(f_pair.first.alive()); }),
        attack_towers.end()
    );
}