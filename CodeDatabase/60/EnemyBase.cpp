#include <EnemyBase.hpp>
#include <random>
#include <PlayerTrap.hpp>
#include <Floor.hpp>
#include <ncurses.h>

EnemyBase::EnemyBase(int x, int y, int level) : LivingEntity(x, y, Entity::Type::ENEMY_BASE, EN_BASE_HP + (level - 1) * 25, 50 + (level - 1) * 50), level(level) {}

void EnemyBase::spawnEnemy(std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, const ScreenSize* screen, int height, int width) {
    int offsets[][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, 
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    int spawnX = -1, spawnY = -1; 
    int spawnWorldX = -1, spawnWorldY = -1; 
    std::shared_ptr<Entity> entityOnSpawnPoint = nullptr;

    for (auto& offset : offsets) {
        int checkScreenX = x + offset[1];
        int checkScreenY = y + offset[0];

        int checkWorldY = checkScreenY - screen->yMax / 2 + height / 2;
        int checkWorldX = checkScreenX - screen->xMax / 2 + width / 2;

        if (checkWorldY >= 0 && checkWorldY < height && checkWorldX >= 0 && checkWorldX < width) {
            auto potentialCellEntity = ceils[checkWorldY][checkWorldX].getEntity();
            Entity::Type type = potentialCellEntity->getType();

            if (type == Entity::Type::FLOOR || type == Entity::Type::PLAYER_TRAP) {
                spawnX = checkScreenX;
                spawnY = checkScreenY;
                spawnWorldX = checkWorldX;
                spawnWorldY = checkWorldY;
                entityOnSpawnPoint = potentialCellEntity;
                break; 
            }
        }
    }

    if (spawnX == -1) {
        return;
    }

    auto& targetCell = ceils[spawnWorldY][spawnWorldX];

    auto newEnemy = std::make_shared<Enemy>(spawnX, spawnY, level);

    if (entityOnSpawnPoint->getType() == Entity::Type::PLAYER_TRAP) {

        auto trap = std::static_pointer_cast<PlayerTrap>(entityOnSpawnPoint);

        newEnemy->takeDamage(trap->getDamage());

        if (newEnemy->getHealth() > 0) {

            targetCell.setEntity(newEnemy);
            entities.push_back(newEnemy);
        } else {

            targetCell.setEntity(std::make_shared<Floor>(spawnX, spawnY));
        }
    } 

    else if (entityOnSpawnPoint->getType() == Entity::Type::FLOOR) {
        targetCell.setEntity(newEnemy);
        entities.push_back(newEnemy);
    }

    targetCell.getEntity()->draw(); 

    refresh(); 
}

void EnemyBase::draw() {
    mvaddch(y, x, sprite);
}

void EnemyBase::destroyed() {
    health = 0;
}

int EnemyBase::getExp() const {
    return exp;
}

int EnemyBase::getLvl() const {
    return level;
}