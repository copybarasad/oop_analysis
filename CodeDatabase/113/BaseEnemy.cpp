#include "BaseEnemy.h"
#include "Player.h"

bool BaseEnemy::canAttackPlayer(const Player& player) const {
    int dx = std::abs(getPositionX() - player.getPositionX());
    int dy = std::abs(getPositionY() - player.getPositionY());
    return (dx <= 1 && dy <= 1);  // Атака в соседних клетках
}