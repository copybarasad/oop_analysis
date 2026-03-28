#include "GameClass/EnemyController.hpp"

EnemyController::EnemyController(int level, MapController& map) {
    loadLevel(level, map);
}


void EnemyController::loadLevel(int lvl, MapController& map) {
    enemies.clear();

    std::vector<EnemyDTO> enemies = LM.loadLevelEnemies(lvl);
    for (auto& e : enemies) {
        Enemy enemy = Enemy();
        enemy.setDamagePoints(e.dmg);
        enemy.setDisabled(e.disabled);
        enemy.setHealthPoints(e.hp);
        enemy.setMaxHP(e.maxHP);
        enemy.setPosition(e.coord);
        addEnemy(enemy, map);
    }
}

std::vector<EnemyDTO> EnemyController::toDTO() {
    std::vector<EnemyDTO> res;

    for (auto& e : enemies) {
        EnemyDTO dto;
        dto.coord = e.getPosition();
        dto.disabled = e.isDisabled();
        dto.dmg = e.getDamagePoints();
        dto.hp = e.getHealthPoints();
        dto.maxHP = e.getMaxHP();
        res.push_back(dto);
    }
    
    return res;
}



void EnemyController::moveEnemy(Enemy& enemy, Coordinates oldPos, Coordinates newPos, MapController& map) {
    map.getField(oldPos)->toInitStatus();
    map.getField(newPos)->setStatus(FieldType::ENEMY);
    enemy.setPosition(newPos);
}

void EnemyController::makeAllStep(MapController& map, Player& player, StatisticDTO& stats) {
    removeDeadEnemies(map, stats);

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->isDisabled()) {
            it->changeDisabled();
            ++it;
            continue;
        }

        Coordinates enemyPos = it->getPosition();
        Coordinates choice = findBestWay(enemyPos, player.getPosition(), map.getAllowedSteps(enemyPos, FieldType::ENEMY));
        Coordinates newPos = {enemyPos.x + choice.x, enemyPos.y + choice.y};

        FieldType stepField = map.getField(enemyPos.x + choice.x, enemyPos.y + choice.y)->getStatus();


        switch (stepField)
        {
        case FieldType::FIELD_SLOW:
            moveEnemy(*it, enemyPos, newPos, map);
            it->changeDisabled();
            break;
        case FieldType::PLAYER:
            player.minusHealthPoints(it->getDamagePoints());
            break;
        default:
            moveEnemy(*it, enemyPos, newPos, map);
        }
        ++it;
    }
    removeDeadEnemies(map, stats);
    needMove = false;
}


Coordinates EnemyController::findBestWay(Coordinates enemyPos, Coordinates playerPos, std::vector<Coordinates> ways) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist100(1, 100);

    Coordinates choice = {0, 0};
    int bestWay = std::abs(playerPos.x - enemyPos.x) + std::abs(playerPos.y - enemyPos.y);
    for (auto it = ways.begin(); it != ways.end(); ) {
        int way = std::abs(playerPos.x - enemyPos.x - it->x) + std::abs(playerPos.y - enemyPos.y - it->y);
        if (way < bestWay) {
            choice = {it->x, it->y};
            bestWay = way;
        }
        if (way == bestWay && dist100(rng) > 50) {
            choice = {it->x, it->y};
        }
        it++;
    }
    return choice;
}


void EnemyController::addEnemy(Enemy& enemy, MapController& map) {
    map.getField(enemy.getPosition())->setStatus(FieldType::ENEMY);
    enemies.push_back(enemy);
}

void EnemyController::removeDeadEnemies(MapController& map, StatisticDTO& stats) {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->getHealthPoints() <= 0) {
            map.getField(it->getPosition())->toInitStatus();
            it = enemies.erase(it);
            stats.points += 50;
            stats.bonus = true;
        } else {
            ++it;
        }
    }
}


Enemy* EnemyController::getEnemy(Coordinates position) {
    for (Enemy& enemy : enemies) {
        if (enemy.getPosition() == position) {
            return &enemy;
        }
    }
    return nullptr;
}


void EnemyController::makeActive() {
    needMove = true;
}

bool EnemyController::isActive() {
    return needMove;
}