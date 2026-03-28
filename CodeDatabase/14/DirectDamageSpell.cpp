#include "DirectDamageSpell.hpp"
#include "GameBoard.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "EnemyBuilding.hpp"
#include "EnemyTower.hpp"
#include <cmath>

bool DirectDamageSpell::cast(GameBoard& board, Player& player, int targetX, int targetY) {
    int dx = player.getX() - targetX;
    int dy = player.getY() - targetY;
    double distance = std::sqrt(dx * dx + dy * dy);

    double effective_range = DIRECT_DAMAGE_SPELL_RANGE + (player.getEnhancementStacks() * DIRECT_DAMAGE_SPELL_RANGE_BONUS);

    if (distance > effective_range) {
        board.addLogMessage("Target is out of range for Direct Damage.");
        return false;
    }

    Entity* target = board.getEntityAt(targetX, targetY);
    if (!target) {
        board.addLogMessage("You cast Direct Damage on an empty cell.");
        return false;
    }

    int total_damage = DIRECT_DAMAGE_SPELL_DAMAGE;

    EntityType type = target->getEntityType();

    if (type == EntityType::ENEMY || type == EntityType::BUILDING || type == EntityType::TOWER) {
        target->takeDamage(total_damage);
        
        std::string name = "Target";
        if (type == EntityType::ENEMY) name = "Enemy";
        if (type == EntityType::BUILDING) name = "Building";
        if (type == EntityType::TOWER) name = "Tower";

        board.addLogMessage(name + " takes " + std::to_string(total_damage) + " spell damage! HP: " + std::to_string(target->getHealth()));

        if (target->getHealth() <= 0) {
            board.addLogMessage(name + " destroyed by a spell!");

            if (type == EntityType::ENEMY) {
                player.addScore(SCORE_PER_KILL);
                board.removeEnemy(static_cast<Enemy*>(target));
            } else if (type == EntityType::BUILDING) {
                player.addScore(SCORE_PER_BUILDING_DESTROYED);
                board.removeBuilding(static_cast<EnemyBuilding*>(target));
            } else if (type == EntityType::TOWER) {
                player.addScore(SCORE_PER_TOWER_DESTROYED);
                board.removeTower(static_cast<EnemyTower*>(target));
            }
            player.incrementKillCount();
        }
    } else {
        board.addLogMessage("This spell only affects enemies, buildings and towers.");
        return false;
    }

    player.clearEnhancementStacks();
    return true;
}

std::string DirectDamageSpell::getName() const { return "Direct Damage"; }
std::string DirectDamageSpell::getDescription() const { return "Deals " + std::to_string(DIRECT_DAMAGE_SPELL_DAMAGE) + " dmg to one target."; }
double DirectDamageSpell::getRange() const { return DIRECT_DAMAGE_SPELL_RANGE; }