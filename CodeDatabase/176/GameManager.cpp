#include "GameManager.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhanceSpell.h"
#include <iostream>
#include <random>

GameManager::GameManager(int width, int height, int level)
    : field(std::make_unique<GameField>(width, height)),
      player(std::make_unique<Player>(100, 30, 15, 50, 6)) {
    playerPosition = {1, 1};
    
    int enemyHealth = level * 10;
    int enemyDamage = level * 5;
    
    addEnemyBuilding(5, 5, 3, enemyHealth, enemyDamage);
    addEnemyTower(8, 8, 1);
}

bool GameManager::movePlayer(Direction direction) {
    if (player->getIsSlowed()) {
        std::cout << "Player is slowed and skips this turn!\n";
        player->setSlowed(false);
        return true;
    }

    int newX = playerPosition.first;
    int newY = playerPosition.second;

    switch (direction) {
        case Direction::UP: newY--; break;
        case Direction::DOWN: newY++; break;
        case Direction::LEFT: newX--; break;
        case Direction::RIGHT: newX++; break;
    }

    return tryMovePlayer(newX, newY);
}

bool GameManager::tryMovePlayer(int newX, int newY) {
    if (!field->isValidPosition(newX, newY)) {
        std::cout << "Cannot move there - outside the map!\n";
        return false;
    }

    if (hasBuildingAt(newX, newY)) {
        std::cout << "Cannot move there - enemy building!\n";
        return false;
    }

    if (field->hasEnemyAt(newX, newY)) {
        std::cout << "Cannot move there - enemy blocking the way!\n";
        return false;
    }

    if (!field->isCellPassable(newX, newY)) {
        std::cout << "Cannot move there - obstacle!\n";
        return false;
    }

    if (field->getCell(newX, newY).getType() == CellType::SLOW) {
        player->setSlowed(true);
        std::cout << "Stepped on slow cell! Will be slowed next turn.\n";
    }

    playerPosition = {newX, newY};
    std::cout << "Moved to (" << newX << ", " << newY << ")\n";
    return true;
}

bool GameManager::attackEnemy(Direction direction) {
    if (player->getCombatMode() == CombatMode::MELEE) {
        int targetX = playerPosition.first;
        int targetY = playerPosition.second;

        switch (direction) {
            case Direction::UP: targetY--; break;
            case Direction::DOWN: targetY++; break;
            case Direction::LEFT: targetX--; break;
            case Direction::RIGHT: targetX++; break;
        }

        return performMeleeAttack(targetX, targetY);
    } else {
        return performRangedAttack(direction);
    }
}

bool GameManager::performMeleeAttack(int targetX, int targetY) {
    if (!field->isValidPosition(targetX, targetY)) {
        std::cout << "Cannot attack there - outside the map!\n";
        return false;
    }

    if (!field->hasEnemyAt(targetX, targetY)) {
        std::cout << "No enemy in adjacent cell to melee attack!\n";
        return false;
    }

    auto enemy = field->getEnemyAt(targetX, targetY);
    int playerDamage = player->getDamage();

    std::cout << "MELEE ATTACK! Deals " << playerDamage << " damage to enemy!\n";

    Enemy updatedEnemy = *enemy;
    updatedEnemy.takeDamage(playerDamage);

    field->removeEnemyAt(targetX, targetY);

    if (!updatedEnemy.isAlive()) {
        player->addScore(10);
        player->addEnemyKilled();
        std::cout << "Enemy defeated! +10 points\n";
        return true;
    } else {
        field->addEnemyAt(targetX, targetY, updatedEnemy);
        std::cout << "Enemy survived with " << updatedEnemy.getHealth() << " health!\n";
        return true;
    }
}

bool GameManager::performRangedAttack(Direction direction) {
    int currentX = playerPosition.first;
    int currentY = playerPosition.second;
    for (int range = 1; range <= 2; range++) {
        int targetX = playerPosition.first;
        int targetY = playerPosition.second;

        switch (direction) {
            case Direction::UP: targetY -= range; break;
            case Direction::DOWN: targetY += range; break;
            case Direction::LEFT: targetX -= range; break;
            case Direction::RIGHT: targetX += range; break;
        }

        if (!field->isValidPosition(targetX, targetY)) {
            break;
        }

        if (field->hasEnemyAt(targetX, targetY)) {
            auto enemy = field->getEnemyAt(targetX, targetY);
            int playerDamage = 15;

            std::cout << "RANGED ATTACK! Deals " << playerDamage << " damage to enemy at ("
                      << targetX << ", " << targetY << ")!\n";

            Enemy currentEnemy = *enemy;
            currentEnemy.takeDamage(playerDamage);

            field->removeEnemyAt(targetX, targetY);

            if (!currentEnemy.isAlive()) {
                player->addScore(10);
                player->addEnemyKilled();
                std::cout << "Enemy defeated! +10 points\n";
            } else {
                field->addEnemyAt(targetX, targetY, currentEnemy);
            }
            return true;
        }

        if (!field->isCellPassable(targetX, targetY)) {
            std::cout << "Attack blocked by obstacle at (" << targetX << "," << targetY << ")!\n";
            return false;
        }
    }

    std::cout << "No enemy in range to ranged attack! (max range: 2 cells)\n";
    return false;
}

void GameManager::switchPlayerCombatMode() {
    player->switchCombatMode();
    std::cout << "Switched to "
              << (player->getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED")
              << " combat! Damage: " << player->getDamage() << "\n";
}

void GameManager::spawnNewEnemies() {
    std::vector<std::pair<int, int>> newEnemiesThisTurn;

    for (size_t i = 0; i < buildings.size(); ++i) {
        buildings[i]->update();
        if (buildings[i]->canSpawnEnemy()) {
            auto newEnemy = buildings[i]->spawnEnemy();
            if (newEnemy) {
                auto adjacent = field->getAdjacentPositions(buildingPositions[i].first, buildingPositions[i].second);
                bool spawned = false;
                for (const auto& pos : adjacent) {
                    if (field->isCellPassable(pos.first, pos.second) &&
                        !(pos.first == playerPosition.first && pos.second == playerPosition.second) &&
                        !field->hasEnemyAt(pos.first, pos.second) &&
                        !hasBuildingAt(pos.first, pos.second)) {
                        field->addEnemyAt(pos.first, pos.second, *newEnemy);
                        std::cout << "New enemy spawned from building at (" << pos.first << ", " << pos.second << ")!\n";
                        newEnemiesThisTurn.push_back(pos);
                        spawned = true;
                        break;
                    }
                }
                if (!spawned) {
                    std::cout << "Building tried to spawn enemy but no space available.\n";
                }
            }
        }
    }

}


void GameManager::updateAllEnemies(const std::vector<std::pair<int, int>>& newEnemiesThisTurn) {
    std::vector<std::pair<int, int>> allEnemies;
    for (int y = 0; y < field->getHeight(); ++y) {
        for (int x = 0; x < field->getWidth(); ++x) {
            if (field->hasEnemyAt(x, y)) {
                allEnemies.emplace_back(x, y);
            }
        }
    }

    std::vector<std::pair<int, int>> enemiesThatAttacked;
    for (const auto& pos : allEnemies) {
        bool isNewEnemy = false;
        for (const auto& newPos : newEnemiesThisTurn) {
            if (newPos.first == pos.first && newPos.second == pos.second) {
                isNewEnemy = true;
                break;
            }
        }
        if (isNewEnemy) continue;

        auto adjacent = field->getAdjacentPositions(pos.first, pos.second);
        for (const auto& adjPos : adjacent) {
            if (adjPos.first == playerPosition.first && adjPos.second == playerPosition.second) {
                auto enemy = field->getEnemyAt(pos.first, pos.second);
                if (enemy) {
                    int enemyDamage = enemy->getDamage();
                    std::cout << "ENEMY at (" << pos.first << "," << pos.second << ") ATTACKS! Deals "
                              << enemyDamage << " damage to player!\n";
                    player->takeDamage(enemyDamage);
                    enemiesThatAttacked.push_back(pos);
                }
                break;
            }
        }
    }

    for (const auto& pos : allEnemies) {
        bool isNewEnemy = false;
        for (const auto& newPos : newEnemiesThisTurn) {
            if (newPos.first == pos.first && newPos.second == pos.second) {
                isNewEnemy = true;
                break;
            }
        }
        if (isNewEnemy) continue;

        bool attackedThisTurn = false;
        for (const auto& attackedPos : enemiesThatAttacked) {
            if (attackedPos.first == pos.first && attackedPos.second == pos.second) {
                attackedThisTurn = true;
                break;
            }
        }
        if (attackedThisTurn) continue;

        if (!field->hasEnemyAt(pos.first, pos.second)) {
            continue;
        }

        auto enemy = field->getEnemyAt(pos.first, pos.second);
        if (!enemy) continue;

        auto adjacent = field->getAdjacentPositions(pos.first, pos.second);
        std::vector<std::pair<int, int>> possibleMoves;

        for (const auto& adjPos : adjacent) {
            if (field->isValidPosition(adjPos.first, adjPos.second) &&
                !(adjPos.first == playerPosition.first && adjPos.second == playerPosition.second) &&
                !field->hasEnemyAt(adjPos.first, adjPos.second) &&
                !hasBuildingAt(adjPos.first, adjPos.second) &&
                field->getCell(adjPos.first, adjPos.second).getType() != CellType::OBSTACLE &&
                field->isCellPassable(adjPos.first, adjPos.second)) {
                possibleMoves.push_back(adjPos);
            }
        }

        if (!possibleMoves.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, possibleMoves.size() - 1);
            auto newPos = possibleMoves[dis(gen)];

            field->removeEnemyAt(pos.first, pos.second);
            field->addEnemyAt(newPos.first, newPos.second, *enemy);
            std::cout << "Enemy moved from (" << pos.first << "," << pos.second << ") to ("
                      << newPos.first << "," << newPos.second << ")\n";
        }
    }
}

void GameManager::updateTowers() {
    for (auto& tower : towers) {
        tower->update();
        tower->tryAttackPlayer(*this);
    }
}

bool GameManager::castSpell(int spellIndex, int targetX, int targetY) {
    if (isGameOver()) {
        std::cout << "Game over! Cannot cast spells.\n";
        return false;
    }

    auto* spellHand = player->getSpellHand();
    if (!spellHand) {
        std::cout << "No spell hand available!\n";
        return false;
    }

    auto* spell = spellHand->getSpell(spellIndex);
    if (!spell) {
        std::cout << "No spell in slot " << (spellIndex + 1) << "!\n";
        return false;
    }

    int manaCost = spell->getManaCost();

    if (player->getMana() < manaCost) {
        std::cout << "Not enough mana! Need " << manaCost
                  << " but have " << player->getMana() << " mana.\n";
        return false;
    }

    bool success = spellHand->castSpell(spellIndex, *this, targetX, targetY, player->getEnhancementManager());
    
    if (success) {
        player->useMana(manaCost);
    }

    return success;
}

void GameManager::addEnemyBuilding(int x, int y, int spawnInterval, int enemyHealth, int enemyDamage) {
    if (field->isValidPosition(x, y) && field->isCellPassable(x, y)) {
        buildingPositions.emplace_back(x, y);
        buildings.push_back(std::make_unique<EnemyBuilding>(spawnInterval, enemyHealth, enemyDamage));
        std::cout << "Enemy building placed at (" << x << ", " << y << ")\n";
    }
}

void GameManager::addEnemyTower(int x, int y, int range) {
    if (field->isValidPosition(x, y)) {
        towers.push_back(std::make_unique<EnemyTower>(x, y, range));
        std::cout << "Enemy tower placed at (" << x << ", " << y << ")\n";
    }
    if (!field->isCellPassable(x, y)) {
        field->setCellType(x, y, CellType::EMPTY);
    }
}

bool GameManager::purchaseSpell(int cost) {
    auto* spellHand = player->getSpellHand();
    if (!spellHand || spellHand->isFull()) {
        std::cout << "Spell hand is full!\n";
        return false;
    }

    if (player->getScore() >= cost) {
        player->addScore(-cost);
        giveNewSpellToPlayer();
        return true;
    }

    std::cout << "Not enough points! Need " << cost << " but have " << player->getScore() << "\n";
    return false;
}

const Player& GameManager::getPlayer() const {
    return *player;
}

std::pair<int, int> GameManager::getPlayerPosition() const {
    return playerPosition;
}

const GameField& GameManager::getField() const {
    return *field;
}

bool GameManager::hasBuildingAt(int x, int y) const {
    for (const auto& pos : buildingPositions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}

bool GameManager::hasTowerAt(int x, int y) const {
    for (const auto& tower : towers) {
        if (tower->getPosX() == x && tower->getPosY() == y) {
            return true;
        }
    }
    return false;
}

bool GameManager::isGameOver() const {
    return !player->isAlive();
}

void GameManager::damageEnemyAt(int x, int y, int damage) {
    if (field->hasEnemyAt(x, y)) {
        auto enemy = field->getEnemyAt(x, y);
        if (enemy) {
            Enemy updatedEnemy = *enemy;
            updatedEnemy.takeDamage(damage);
            
            field->removeEnemyAt(x, y);
            
            if (!updatedEnemy.isAlive()) {
                player->addScore(10);
                player->addEnemyKilled();
                std::cout << "Enemy defeated! +10 points\n";
            } else {
                field->addEnemyAt(x, y, updatedEnemy);
            }
        }
    }
}

bool GameManager::damageEnemiesInArea(int centerX, int centerY, int areaSize, int damage) {
    int hits = 0;
    int halfSize = areaSize / 2;
    
    for (int y = centerY - halfSize; y <= centerY + halfSize; ++y) {
        for (int x = centerX - halfSize; x <= centerX + halfSize; ++x) {
            if (field->isValidPosition(x, y) && field->hasEnemyAt(x, y)) {
                damageEnemyAt(x, y, damage);
                hits++;
            }
        }
    }
    
    return hits > 0;
}

void GameManager::checkSpellRewards() {
    auto* spellHand = player->getSpellHand();
    if (!spellHand || spellHand->isFull()) return;

    int currentKills = player->getEnemiesKilled();

    if (currentKills >= 3) {
        static int spellsGiven = 1;
        
        if (currentKills / 3 >= spellsGiven) {
            giveNewSpellToPlayer();
            spellsGiven = currentKills / 3 + 1;
            std::cout << "=== REWARD: New spell unlocked! ===\n";
        }
    }
}

std::unique_ptr<Spell> GameManager::createSpellForSlot(int slotIndex) {
    switch (slotIndex) {
        case 0: return std::make_unique<DirectDamageSpell>(20, 3, 10);
        case 1: return std::make_unique<AreaDamageSpell>(15, 3, 2, 15);
        case 2: return std::make_unique<TrapSpell>(20, 2, 8);
        case 3: return std::make_unique<SummonSpell>(1, 2, 20);
        case 4: return std::make_unique<EnhanceSpell>(5);
        default: return std::make_unique<DirectDamageSpell>(20, 3, 10);
    }
}

void GameManager::giveNewSpellToPlayer() {
    auto* spellHand = player->getSpellHand();
    if (!spellHand || spellHand->isFull()) return;

    for (size_t i = 0; i < spellHand->getMaxSize(); ++i) {
        if (!spellHand->getSpell(i)) {
            auto newSpell = createSpellForSlot(i);
            if (spellHand->addSpell(std::move(newSpell))) {
                std::cout << "Earned new spell in slot " << (i + 1) << "!\n";
                break;
            }
        }
    }
}

void GameManager::placeTrapAt(int x, int y, int damage) {
    if (field->isValidPosition(x, y) && field->isCellPassable(x, y)) {
        const_cast<GameField&>(*field).placeTrap(x, y);
    }
}

int GameManager::summonAlliesNearPlayer(int count) {
    int summoned = 0;
    auto playerPos = getPlayerPosition();
    
    for (int y = playerPos.second - 1; y <= playerPos.second + 1 && summoned < count; ++y) {
        for (int x = playerPos.first - 1; x <= playerPos.first + 1 && summoned < count; ++x) {
            if (field->isValidPosition(x, y) &&
                field->isCellPassable(x, y) &&
                !field->hasEnemyAt(x, y) &&
                !field->hasAllyAt(x, y) &&
                !(x == playerPos.first && y == playerPos.second)) {
                
                const_cast<GameField&>(*field).placeAlly(x, y);
                summoned++;
            }
        }
    }
    return summoned;
}

void GameManager::updateTraps() {
    std::vector<std::pair<int, int>> trapsToRemove;

    const auto& trapPositions = field->getTrapPositions();
    for (const auto& trapPos : trapPositions) {
        int x = trapPos.first;
        int y = trapPos.second;
        
        if (field->hasEnemyAt(x, y)) {
            auto enemy = field->getEnemyAt(x, y);
            if (enemy) {
                std::cout << "Trap at (" << x << ", " << y << ") triggers on enemy!\n";
                damageEnemyAt(x, y, 20);
                trapsToRemove.push_back({x, y});
            }
        }
    }
    
    for (const auto& trapPos : trapsToRemove) {
        field->triggerTrap(trapPos.first, trapPos.second);
    }
}

void GameManager::updateAllies() {
    std::vector<std::pair<int, int>> allyPositions = field->getAllyPositions();
    
    for (const auto& allyPos : allyPositions) {
        int allyX = allyPos.first;
        int allyY = allyPos.second;

        std::pair<int, int> nearestEnemy = {-1, -1};
        int minDistance = std::numeric_limits<int>::max();
        
        for (int y = 0; y < field->getHeight(); ++y) {
            for (int x = 0; x < field->getWidth(); ++x) {
                if (field->hasEnemyAt(x, y)) {
                    int distance = std::abs(x - allyX) + std::abs(y - allyY);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestEnemy = {x, y};
                    }
                }
            }
        }
        
        if (nearestEnemy.first != -1) {
            int newX = allyX;
            int newY = allyY;
            
            if (std::abs(nearestEnemy.first - allyX) > std::abs(nearestEnemy.second - allyY)) {
                newX += (nearestEnemy.first > allyX) ? 1 : -1;
            } else {
                newY += (nearestEnemy.second > allyY) ? 1 : -1;
            }

            if (field->isValidPosition(newX, newY) &&
                field->isCellPassable(newX, newY) &&
                !field->hasEnemyAt(newX, newY) &&
                !field->hasAllyAt(newX, newY) &&
                !(newX == playerPosition.first && newY == playerPosition.second)) {
                
                field->removeAlly(allyX, allyY);
                field->placeAlly(newX, newY);
                std::cout << "Ally moves from (" << allyX << "," << allyY << ") to ("
                          << newX << "," << newY << ")\n";
            }
        }
    }
}

void GameManager::updateGame() {
    player->regenerateMana();
    updateAllies();
    updateTraps();
    updateAllEnemies({});
    spawnNewEnemies();
    updateTowers();
    checkSpellRewards();
}

Player& GameManager::getPlayer() {
    return *player;
}

GameField& GameManager::getField() {
    return *field;
}

void GameManager::setPlayerPosition(int x, int y) {
    if (field->isValidPosition(x, y) && field->isCellPassable(x, y)) {
        playerPosition = {x, y};
    }
}
