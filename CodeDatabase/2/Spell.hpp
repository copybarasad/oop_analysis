#pragma once

#include "Enemy.hpp"
#include "Player.hpp"
#include "EnemyAttackTower.hpp"
#include "EnemyTower.hpp"
#include "Ally.hpp"
#include <vector>
#include <string>

enum SpellType {
    UNKNOWN_SPELL = 0,
    DIRECT_DAMAGE = 1,
    SPLASH_DAMAGE = 2,
    TRAP = 3,
    SUMMON_ALLY = 4,
    ENHANCEMENT = 5
};

class Spell {
public:
    virtual bool cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) = 0;

    virtual int get_range() const = 0;
    virtual std::string get_name() const = 0;
    virtual bool requires_target() const = 0;
    virtual int get_mana_cost() const = 0;
    virtual ~Spell() = default;
};