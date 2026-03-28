#include "EnemyBuilding.h"
#include "Field.h"
#include "Player.h"
#include <cmath>

EnemyBuilding::EnemyBuilding(int hp,int attackRadius, int attackCooldown)
        : Entity(hp, 0, "EnemyBuilding"),
          attackRadius_(attackRadius), attackCooldown_(attackCooldown), turnsUntilAttack_(0) {}

std::shared_ptr<Entity> EnemyBuilding::clone() const { return std::make_shared<EnemyBuilding>(*this); }

void EnemyBuilding::decrementTimer() {
    if (turnsUntilAttack_ > 0) --turnsUntilAttack_;
}

static int distMan(const Coord& a, const Coord& b) { return std::abs(a.row - b.row) + std::abs(a.col - b.col); }

bool EnemyBuilding::tryAttackPlayer(Field& field, Coord pos) {
    auto pc = field.findPlayerCoord();
    if (!pc) return false;
    if (distMan(*pc, pos) > attackRadius_) return false;
    if (turnsUntilAttack_ > 0) return false;
    auto playerCell = field.cellAt(*pc);
    auto ent = playerCell.getEntity();
    if (ent && dynamic_cast<Player*>(ent.get())) {
        ent->takeDamage(2); // weakened attack
        turnsUntilAttack_ = attackCooldown_;
        return true;
    }
    return false;
}

void EnemyBuilding::onTurn(Field& field, const Coord& pos) {
}

void EnemyBuilding::setTurnsUntilAttack(int v) { turnsUntilAttack_ = v; }
int EnemyBuilding::turnsUntilAttack() const { return turnsUntilAttack_; }
