#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <string>

class CombatSystem {
private:
    static const std::vector<std::pair<int, int> > DIRECTIONS;

public:
    CombatSystem();

    void performMeleeAttack(Player &player, const Field &field,
                            std::vector<Enemy> &enemies, int dx, int dy,
                            std::function<void(const std::string &)> addMessage,
                            std::function<void()> addScore);

    void performRangedAttack(Player &player, const Field &field,
                             std::vector<Enemy> &enemies, int dx, int dy,
                             std::function<void(const std::string &)> addMessage,
                             std::function<void()> addScore);

    void handleEnemyAttack(Enemy &enemy, Player &player,
                           std::function<void(const std::string &)> addMessage);
};

#endif
