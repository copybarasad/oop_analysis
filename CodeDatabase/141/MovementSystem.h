#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include <vector>
#include <functional>

class MovementSystem {
private:
    static const std::vector<std::pair<int, int> > DIRECTIONS;

public:
    MovementSystem();

    bool movePlayer(Player &player, int dx, int dy, const Field &field,
                    const std::vector<Enemy> &enemies,
                    const std::vector<EnemyBuilding> &buildings,
                    const std::vector<EnemyTower> &towers,
                    const std::vector<Ally> &allies,
                    std::function<void(const std::string &)> addMessage);

    void moveEnemy(Enemy &enemy, const Field &field, Player &player,
                   const std::vector<Enemy> &otherEnemies,
                   const std::vector<EnemyBuilding> &buildings,
                   const std::vector<EnemyTower> &towers,
                   const std::vector<Ally> &allies,
                   std::function<void(const std::string &)> addMessage,
                   std::function<void(Enemy &, Player &)> attackCallback);

    void moveAlly(Ally &ally, const Field &field,
                  const std::vector<Enemy> &enemies,
                  const std::vector<EnemyBuilding> &buildings,
                  const std::vector<EnemyTower> &towers,
                  const std::vector<Ally> &otherAllies,
                  const Player &player);

    bool isPositionFree(int x, int y, const Field &field,
                        const Player &player,
                        const std::vector<Enemy> &enemies,
                        const std::vector<EnemyBuilding> &buildings,
                        const std::vector<EnemyTower> &towers,
                        const std::vector<Ally> &allies) const;
};

#endif
