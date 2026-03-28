#include "AIEnemy.h"
#include "combat_system.h"


bool AIEnemy::choice_attack_go(Enemy& enemy, Player& gamer) {
    GameObject* fighter = &enemy;
    GameObject* target = &gamer;
    int distance_to_target = std::max(std::abs(fighter->getX() - target->getX()), std::abs(fighter->getY() - target->getY()));
    if (distance_to_target <= fighter->getRange()) {
        CombatSystem::attack(fighter, target);
        return true;
    }
    return false;
}



std::pair<int, int> AIEnemy::alg_finding_way(Enemy& enemy, Player& gamer) {
    int x_now = enemy.getX();
    int y_now = enemy.getY();
    int x_player = gamer.getX();
    int y_player = gamer.getY();
    int distance_x = x_player - x_now;
    int distance_y = y_player - y_now;
    if (distance_x != 0 && distance_y != 0) {
        if (distance_x > 0 && distance_y > 0) {
            return {x_now + 1, y_now + 1};
        } else if (distance_x > 0 && distance_y < 0) {
            return {x_now + 1, y_now -1};
        } else if (distance_x < 0 && distance_y > 0) {
            return {x_now -1, y_now +1};
        } else {
            return {x_now -1, y_now -1};
        }
    } else if (distance_x != 0) {
        if (distance_x > 0) {
            return {x_now + 1, y_now };
        } else {
            return {x_now -1, y_now };
        }
    } else {
        if (distance_y > 0) {
            return {x_now + 0, y_now + 1};
        } else {
            return {x_now + 0, y_now -1};
        }
    }
    return {0, 0};
}

