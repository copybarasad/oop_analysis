#include "CombatSystem.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Spawner.h"
#include "Field.h"
#include "GameModel.h"

std::pair<bool,bool> CombatSystem::resolveAttack(std::shared_ptr<Entity> attacker,
                                                 std::shared_ptr<Entity> defender,
                                                 const Coord& defenderPos,
                                                 GameModel& model) {
    if (!attacker || !defender) return {false, false};

    defender->takeDamage(attacker->damage());

    bool defenderDied = false;
    bool attackerDied = false;

    if (!defender->isAlive()) {
        defenderDied = true;
        if (dynamic_cast<Enemy*>(defender.get())) {
            if (auto pl = model.player()) {
                if (dynamic_cast<Player*>(attacker.get())) {
                    pl->addScore(1);
                }
            }
        }
        model.field().removeEntity(defenderPos);
    }

    return {defenderDied, attackerDied};
}

bool CombatSystem::resolveTrap(std::shared_ptr<Entity> defender,
                               std::shared_ptr<Entity> trap,
                               const Coord& trapPos,
                               GameModel& model) {
    if (!defender || !trap) return false;
    defender->takeDamage(trap->damage());
    bool died = false;
    if (!defender->isAlive()) {
        died = true;
    }
    model.field().removeEntity(trapPos);
    return died;
}
