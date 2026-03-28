#include "CombatService.hpp"

CombatService::CombatResult CombatService::performCloseFight (Player& player, Entity& enemy) {
    CombatResult result;
    unsigned int playerDamage = player.getMode() == CombatMode::CLOSE ? player.calculateDamage() : 0;
    unsigned int enemyDamage = enemy.calculateDamage();

    unsigned int playerHealth = player.getCurrentHealth() - std::min(enemyDamage, player.getCurrentHealth());
    unsigned int enemyHealth = enemy.getCurrentHealth() - std::min(playerDamage, enemy.getCurrentHealth());

    checkEnemy(result, player, enemy, enemyHealth);

    if (!playerHealth) {
        result.destroyedEntities.push_back(player.getID());
    }

    enemy.setCurrentHealth(enemyHealth);
    player.setCurrentHealth(playerHealth);

    return result;
}

CombatService::CombatResult CombatService::performRangedAttack (Player& player, Entity& enemy) {
    CombatResult result;
    if (player.getMode() != CombatMode::RANGED) {
        result.success = false;
        return result;
    }

    unsigned int playerDamage = player.calculateDamage();
    unsigned int enemyHealth = enemy.getCurrentHealth() - std::min(playerDamage, enemy.getCurrentHealth());
    
    checkEnemy(result, player, enemy, enemyHealth);
    
    enemy.setCurrentHealth(enemyHealth);

    return result;
}

CombatService::CombatResult CombatService::performNpcFight (Player& player, Entity& npc, Entity& enemy) {
    CombatResult result;

    unsigned int npcDamage = npc.calculateDamage();
    unsigned int enemyDamage = enemy.calculateDamage();

    unsigned int npcHealth = npc.getCurrentHealth() - std::min(enemyDamage, npc.getCurrentHealth());
    unsigned int enemyHealth = enemy.getCurrentHealth() - std::min(npcDamage, enemy.getCurrentHealth());

    checkEnemy(result, player, enemy, enemyHealth);
    enemy.setCurrentHealth(enemyHealth);
    npc.setCurrentHealth(npcHealth);
    checkNpc(result, npc);

    return result;
}

bool CombatService::canAttack (const Player& player, unsigned int distance) const {
    unsigned int maxRange = player.getWeapon()->getRange();

    return maxRange >= distance;
}

unsigned int CombatService::calculatePoints (unsigned int enemyHealth) {
    std::srand(std::chrono::steady_clock::now().time_since_epoch().count());

    unsigned int points = (std::rand() % 10) * enemyHealth / 100;

    return points;
}

void CombatService::checkEnemy (CombatResult& result, Player& player, Entity& enemy, unsigned int enemyHealth) {
    if (!enemyHealth) {
        unsigned int points = calculatePoints(enemy.getCurrentHealth());
        player.addPoints(points);
        result.destroyedEntities.push_back(enemy.getID());
    }
}

void CombatService::checkNpc (CombatResult& result, Entity& npc) {
    if (!npc.getCurrentHealth()) {
        result.destroyedEntities.push_back(npc.getID());
    }
}