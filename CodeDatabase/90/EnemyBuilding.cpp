#include "EnemyBuilding.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "Field.h"
#include "Player.h"
#include "Random.h"

void EnemyBuilding::update(int fieldWidth, int fieldHeight,
                           const Player &player, const Field &field) {
  if (!isAlive())
    return;
  if (spawnCooldown > 0) {
    spawnCooldown--;
  }
  if (canSpawnEnemy()) {
    std::vector<Position> possibleSpawns;
    for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
        if (dx == 0 && dy == 0)
          continue;
        Position self = getPosition();
        Position newPos(self.getX() + dx, self.getY() + dy);
        if (newPos.getX() >= 0 && newPos.getX() < fieldWidth &&
            newPos.getY() >= 0 && newPos.getY() < fieldHeight &&
            !(newPos == player.getPosition()) &&
            field.getCell(newPos).getType() == CellType::empty) {
          possibleSpawns.push_back(newPos);
        }
      }
    }

    if (!possibleSpawns.empty()) {
      Position spawn =
          possibleSpawns[Random::getInt(0, possibleSpawns.size() - 1)];
      auto newEnemy = std::make_unique<Enemy>(5, 30, spawn);
      if (field.getCell(spawn).getType() == CellType::slowing) {
        newEnemy->setSlowed(true);
      }
      enemies->add(std::move(newEnemy));
      spawnCooldown = maxCooldown;
    }
  }
}