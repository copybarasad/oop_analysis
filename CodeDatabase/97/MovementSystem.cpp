#include "MovementSystem.h"
#include "GameModel.h"
#include "Field.h"
#include "Cell.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Trap.h"
#include "Ally.h"
#include "Spawner.h"
#include <memory>
#include "CombatSystem.h"
#include "GameModel.h"

MovementSystem::MovementSystem(GameModel& model, CombatSystem& combat)
        : model_(model), combat_(combat) {}

std::tuple<bool,bool,bool,bool,bool> MovementSystem::moveEntity(Coord from, Coord to) {
    Field& field = model_.field();

    if (!field.inBounds(from) || !field.inBounds(to)) return {false,false,false,false,false};
    Cell &src = field.cellAt(from);
    Cell &dst = field.cellAt(to);
    if (!src.hasEntity()) return {false,false,false,false,false};
    if (!dst.isPassable()) return {false,false,false,false,false};

    auto mover = src.getEntity();
    auto occupant = dst.getEntity();

    bool playerDied = false;
    bool towerDestroyed = false;
    bool enemyKilled = false;
    bool spawnerKilled = false;

    if (occupant && dynamic_cast<Trap*>(occupant.get()) && dynamic_cast<Enemy*>(mover.get())) {
        bool died = combat_.resolveTrap(mover, occupant, to, model_);
        if (died) {
            src.clearEntity();
            enemyKilled = true;
        }
        return {false, playerDied, towerDestroyed, enemyKilled, spawnerKilled};
    }

    if (occupant && dynamic_cast<Player *>(occupant.get()) && dynamic_cast<Enemy *>(mover.get()) && !dynamic_cast<Ally*>(mover.get())) {
        occupant->takeDamage(mover->damage());
        if (!occupant->isAlive()) playerDied = true;
        return {false, playerDied, false, false, false};
    }

    if (occupant && dynamic_cast<Ally*>(occupant.get()) && dynamic_cast<Enemy *>(mover.get()) && !dynamic_cast<Ally*>(mover.get())) {
        occupant->takeDamage(mover->damage());
        if (!occupant->isAlive()) {
            dst.clearEntity(); // ally died
        }
        return {false,false,false,false,false};
    }

    if (occupant && dynamic_cast<Enemy *>(occupant.get()) && dynamic_cast<Ally *>(mover.get())) {
        if (dynamic_cast<EnemyBuilding*>(occupant.get())) {
            return {false,false,false,false,false};
        }
        occupant->takeDamage(mover->damage());
        if (!occupant->isAlive()) {
            dst.clearEntity();
            dst.setEntity(mover);
            src.clearEntity();
            enemyKilled = true;
            if (dynamic_cast<Player*>(mover.get())) {
                model_.setPlayerCoord(to);
            }
            return {true,false,false,true,false};
        }
        return {false,false,false,false,false};
    }

    if (occupant && dynamic_cast<Enemy *>(occupant.get()) && dynamic_cast<Player *>(mover.get())) {
        auto playerPtr = std::dynamic_pointer_cast<Player>(mover);
        int dmg = playerPtr->getDamageForCurrentMode();
        occupant->takeDamage(dmg);
        if (!occupant->isAlive()) {
            dst.clearEntity();
            dst.setEntity(mover);
            src.clearEntity();
            if (dst.isSlow()) {
                if (auto p = std::dynamic_pointer_cast<Player>(dst.getEntity())) {
                    p->setSkipNextMove(true);
                }
            }
            if (playerPtr) playerPtr->addScore(1);
            enemyKilled = true;
            // update player index
            model_.setPlayerCoord(to);
            return {true,false,false,true,false};
        }
        return {false,false,false,false,false};
    }

    if (occupant && dynamic_cast<Spawner *>(occupant.get()) && dynamic_cast<Player *>(mover.get())) {
        auto playerPtr = std::dynamic_pointer_cast<Player>(mover);
        int dmg = playerPtr->getDamageForCurrentMode();
        occupant->takeDamage(dmg);
        if(!occupant->isAlive()){
            dst.clearEntity();
            dst.setEntity(mover);
            src.clearEntity();
            if(playerPtr) playerPtr->addScore(5);
            spawnerKilled = true;
            model_.setPlayerCoord(to);
            return {true,false,false,false,true};
        }
        return {false,false,false,false,false};
    }

    if (occupant && dynamic_cast<EnemyBuilding *>(occupant.get()) && dynamic_cast<Player *>(mover.get())) {
        auto playerPtr = std::dynamic_pointer_cast<Player>(mover);
        int dmg = playerPtr->getDamageForCurrentMode();
        occupant->takeDamage(dmg);
        if (!occupant->isAlive()) {
            dst.clearEntity();
            dst.setEntity(mover);
            src.clearEntity();
            towerDestroyed = true;
            model_.setPlayerCoord(to);
            return {true,false,true,false,false};
        }
        return {false,false,false,false,false};
    }

    if (!occupant) {
        dst.setEntity(mover);
        src.clearEntity();
        if (dst.isSlow()) {
            if (auto p = std::dynamic_pointer_cast<Player>(dst.getEntity())) {
                p->setSkipNextMove(true);
            }
        }
        if (dynamic_cast<Player*>(mover.get())) {
            model_.setPlayerCoord(to);
        }
        return {true,false,false,false,false};
    }

    return {false,false,false,false,false};
}
