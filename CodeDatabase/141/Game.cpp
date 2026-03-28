#include "Game.h"
#include "GameConstants.h"
#include "LevelManager.h"
#include "SaveSystem.h"
#include <sstream>
#include <algorithm>
#include <string>

void Game::addScoreCallback() {
    // Начисление очков за убийство и выдача новых заклинаний по порогам
    player.addScore(GameConstants::SCORE_PER_KILL);
    enemiesKilled++;
    if (eventBus) {
        eventBus->publish(SpawnEvent{"Очки", 0, 0});
    }

    if (enemiesKilled % GameConstants::ENEMY_KILLS_FOR_NEW_SPELL == 0) {
        addRandomSpellToPlayer();
    }
}

void Game::incrementKillsCallback(int count) {
    enemiesKilled += count;
    player.addScore(GameConstants::SCORE_PER_KILL * count);
}

void Game::handlePlayerMove(int dx, int dy) {
    // Движение игрока с учётом препятствий и занятости клеток
    bool moved = movementSystem.movePlayer(player, dx, dy, field, enemies, buildings, towers, allies,
                                           [this](const std::string &msg) { addMessage(msg); });

    if (moved) {
        int playerX, playerY;
        player.getPosition(playerX, playerY);

        std::stringstream moveLog;
        moveLog << "Перемещение в (" << playerX << ", " << playerY << ")";
        if (field.getCell(playerX, playerY).isSlow()) {
            moveLog << " [МЕДЛ]";
            addMessage("Замедление: следующий ход пропущен, враги ходят дважды");
        }
        addMessage(moveLog.str());
        if (eventBus) {
            eventBus->publish(MoveEvent{"Игрок", playerX - dx, playerY - dy, playerX, playerY});
        }
    }
}

void Game::processEnemiesTurn() {
    // Ход всех живых врагов
    for (auto &enemy: enemies) {
        if (enemy.isAlive()) {
            movementSystem.moveEnemy(enemy, field, player, enemies, buildings, towers, allies,
                                     [this](const std::string &msg) { addMessage(msg); },
                                     [this](Enemy &e, Player &p) {
                                         combatSystem.handleEnemyAttack(e, p,
                                                                        [this](const std::string &msg) {
                                                                            addMessage(msg);
                                                                        });
                                     });
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const Enemy &e) { return !e.isAlive(); }), enemies.end());
}

void Game::processBuildingsTurn() {
    for (auto &building: buildings) {
        building.update();
        if (building.canSpawnEnemy()) {
            spawnEnemyFromBuilding(building);
            building.resetCooldown();
        }
    }
}

void Game::spawnEnemyFromBuilding(EnemyBuilding &building) {
    int spawnX, spawnY;

    bool success = building.trySpawnEnemy(spawnX, spawnY,
                                          [this](int x, int y) {
                                              return field.isValidPosition(x, y) &&
                                                     field.isPassable(x, y) &&
                                                     movementSystem.isPositionFree(
                                                         x, y, field, player, enemies, buildings, towers, allies);
                                          }
    );

    if (success) {
        enemies.push_back(createRandomEnemy(spawnX, spawnY));
        addMessage("Вражеское здание создало врага в (" +
                   std::to_string(spawnX) + ", " + std::to_string(spawnY) + ")");
        if (eventBus) {
            eventBus->publish(SpawnEvent{"Враг", spawnX, spawnY});
        }
    } else {
        addMessage("Здание не смогло создать врага — нет места");
    }
}

void Game::processTraps() {
    // Проверка срабатывания ловушек после хода противников
    for (auto &trap: traps) {
        if (!trap.isActive()) continue;

        for (auto &enemy: enemies) {
            if (!enemy.isAlive()) continue;

            int enemyX, enemyY;
            enemy.getPosition(enemyX, enemyY);

            if (enemyX == trap.getX() && enemyY == trap.getY()) {
                enemy.takeDamage(trap.getDamage());
                trap.trigger();
                addMessage("Ловушка сработала: враг получил " + std::to_string(trap.getDamage()) + " урона");
                if (eventBus) {
                    eventBus->publish(DamageEvent{"Ловушка", "Враг", trap.getDamage()});
                }

                if (!enemy.isAlive()) {
                    addScoreCallback();
                    addMessage("Враг побеждён! +" + std::to_string(GameConstants::SCORE_PER_KILL) + " очков");
                }
                break;
            }
        }
    }

    traps.erase(std::remove_if(traps.begin(), traps.end(),
                               [](const Trap &t) { return !t.isActive(); }), traps.end());
}

void Game::processTowersTurn() {
    for (auto &tower: towers) {
        tower.update();
        if (tower.canAttack() && tower.isPlayerInRange(*this)) {
            tower.attackPlayer(*this);
        }
    }
}

void Game::towerAttackPlayer(int towerX, int towerY, int damage) {
    player.takeDamage(damage);
    addMessage("Башня в (" + std::to_string(towerX) + ", " + std::to_string(towerY) +
               ") нанесла " + std::to_string(damage) + " урона");
    if (eventBus) {
        eventBus->publish(DamageEvent{"Башня", "Игрок", damage});
    }
}

bool Game::isPlayerInRange(int centerX, int centerY, int radius) const {
    int playerX, playerY;
    player.getPosition(playerX, playerY);

    int distanceX = std::abs(playerX - centerX);
    int distanceY = std::abs(playerY - centerY);

    return distanceX <= radius && distanceY <= radius;
}

void Game::processAlliesTurn() {
    for (auto &ally: allies) {
        if (ally.isAlive()) {
            movementSystem.moveAlly(ally, field, enemies, buildings, towers, allies, player);
        }
    }
}


bool Game::isPositionValidAndFree(int x, int y) const {
    return field.isValidPosition(x, y) &&
           field.isPassable(x, y) &&
           movementSystem.isPositionFree(x, y, field, player, enemies, buildings, towers, allies);
}

void Game::getPlayerPosition(int &x, int &y) const {
    player.getPosition(x, y);
}

Field &Game::getField() {
    return field;
}

const Field &Game::getField() const {
    return field;
}

Player &Game::getPlayer() {
    return player;
}

const Player &Game::getPlayer() const {
    return player;
}

std::vector<Enemy> &Game::getEnemies() {
    return enemies;
}

const std::vector<Enemy> &Game::getEnemies() const {
    return enemies;
}

std::vector<EnemyBuilding> &Game::getBuildings() {
    return buildings;
}

const std::vector<EnemyBuilding> &Game::getBuildings() const {
    return buildings;
}

std::vector<Trap> &Game::getTraps() {
    return traps;
}

const std::vector<Trap> &Game::getTraps() const {
    return traps;
}

std::vector<Ally> &Game::getAllies() {
    return allies;
}

const std::vector<Ally> &Game::getAllies() const {
    return allies;
}

const std::vector<EnemyTower> &Game::getTowers() const {
    return towers;
}

void Game::addMessage(const std::string &message) {
    renderSystem.addMessage(message);
    if (eventBus) {
        eventBus->publish(SpellEvent{message});
    }
}

void Game::addScore() {
    addScoreCallback();
}

void Game::incrementKills(int count) {
    incrementKillsCallback(count);
}

void Game::movePlayer(int dx, int dy) {
    handlePlayerMove(dx, dy);
}

void Game::playerAttack(int dx, int dy) {
    if (player.getBattleType() == BattleType::MELEE) {
        combatSystem.performMeleeAttack(player, field, enemies, dx, dy,
                                        [this](const std::string &msg) { addMessage(msg); },
                                        [this]() { addScoreCallback(); });
    } else {
        combatSystem.performRangedAttack(player, field, enemies, dx, dy,
                                         [this](const std::string &msg) { addMessage(msg); },
                                         [this]() { addScoreCallback(); });
    }
}

void Game::switchWeapon() {
    player.switchBattleType();
    addMessage("Переключено на " + std::string(player.getBattleType() == BattleType::MELEE ? "БЛИЖНИЙ" : "ДАЛЬНИЙ"));
}

void Game::castSpellAt(int spellIndex, int dx, int dy) {
    if (spellIndex < 0 || spellIndex >= player.getHand().getSize()) {
        addMessage("Неверный номер заклинания");
        return;
    }
    Spell *spell = player.getHand().getSpell(spellIndex);
    if (!spell) {
        addMessage("В ячейке нет заклинания");
        return;
    }
    std::string spellName = spell->getName();

    if (!spell->requiresTarget()) {
        bool success = spellSystem.castSpell(spell, *this);
        if (success) {
            player.getHand().removeSpell(spellIndex);
            addMessage("Заклинание применено: " + spellName);
        } else {
            addMessage("Не удалось применить заклинание");
        }
        return;
    }

    int playerX, playerY;
    player.getPosition(playerX, playerY);
    int targetX = playerX + dx * spell->getRange();
    int targetY = playerY + dy * spell->getRange();

    bool success = spellSystem.castSpellAt(spell, *this, targetX, targetY);
    if (success) {
        player.getHand().removeSpell(spellIndex);
        addMessage("Заклинание применено: " + spellName);
        if (eventBus) {
            eventBus->publish(SpellEvent{"Применено заклинание: " + spellName});
        }
    } else {
        addMessage("Не удалось применить заклинание");
    }
}

void Game::saveTo(const std::string &path) {
    SaveSystem::saveGame(*this, path);
}

void Game::loadFrom(const std::string &path) {
    SaveSystem::loadGame(*this, path);
}

void Game::afterTurn() {
    if (LevelManager::isLevelComplete(*this)) {
        handleLevelComplete();
        if (!gameRunning) return;
    }

    if (player.shouldSkipTurn()) {
        addMessage("Игрок замедлен и пропускает ход");
        player.setSkipNextTurn(false);
        processAlliesTurn();
        for (int i = 0; i < 2; ++i) {
            processEnemiesTurn();
            processBuildingsTurn();
            processTowersTurn();
            processTraps();
        }
    } else {
        processAlliesTurn();
        processEnemiesTurn();
        processBuildingsTurn();
        processTowersTurn();
        processTraps();
    }
}

int Game::getEnemiesKilled() const {
    return enemiesKilled;
}

int Game::getScoreTarget() const {
    return LevelManager::getScoreTarget(currentLevel);
}

void Game::setEventBus(EventBus *bus) {
    eventBus = bus;
}

void Game::setUpgradeSelector(const std::function<int(const std::vector<UpgradeOption> &)> &selector) {
    upgradeSelector = selector;
}
