#include "Game.h"
#include "GameConstants.h"
#include "SaveSystem.h"
#include "SaveException.h"
#include "FileNotFoundException.h"
#include "InvalidSaveFileException.h"
#include "LoadException.h"
#include "SpellFactory.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>

int Game::getCurrentLevel() const {
    return currentLevel;
}

void Game::setCurrentLevel(int level) {
    if (level < 1) {
        throw std::invalid_argument("Уровень должен быть не меньше 1");
    }
    currentLevel = level;
}

int Game::getFieldWidth() const {
    return field.getWidth();
}

int Game::getFieldHeight() const {
    return field.getHeight();
}

void Game::setFieldSize(int width, int height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Размеры поля должны быть положительными");
    }
    field = Field(width, height);
    int playerX, playerY;
    player.getPosition(playerX, playerY);
    if (playerX >= width) playerX = width - 1;
    if (playerY >= height) playerY = height - 1;
    if (playerX < 0) playerX = 0;
    if (playerY < 0) playerY = 0;
    player.setPosition(playerX, playerY);
}

void Game::setEnemiesKilled(int count) {
    if (count < 0) {
        throw std::invalid_argument("Число убитых врагов не может быть отрицательным");
    }
    enemiesKilled = count;
}

int Game::getPlayerHealth() const {
    return player.getHealth();
}

int Game::getPlayerMeleeDamage() const {
    return player.getMeleeDamage();
}

int Game::getPlayerRangedDamage() const {
    return player.getRangedDamage();
}

int Game::getPlayerScore() const {
    return player.getScore();
}

BattleType Game::getPlayerBattleType() const {
    return player.getBattleType();
}

bool Game::getPlayerSkipTurn() const {
    return player.shouldSkipTurn();
}

void Game::setPlayerState(int x, int y, int health, int meleeDamage, int rangedDamage, int score, BattleType battleType,
                          bool skipTurn) {
    if (health < 0 || meleeDamage <= 0 || rangedDamage <= 0 || score < 0) {
        throw std::invalid_argument("Некорректные параметры игрока");
    }
    player.setPosition(x, y);
    player.setHealth(health);
    player.setMeleeDamage(meleeDamage);
    player.setRangedDamage(rangedDamage);
    player.setScore(score);
    player.setBattleType(battleType);
    player.setSkipNextTurn(skipTurn);
}

const Hand &Game::getPlayerHand() const {
    return player.getHand();
}

void Game::clearPlayerHand() {
    Hand &hand = player.getHand();
    while (!hand.isEmpty()) {
        hand.removeSpell(0);
    }
}

void Game::addSpellToPlayer(Spell *spell) {
    if (!player.getHand().addSpell(spell)) {
        delete spell;
        throw std::runtime_error("Не удалось добавить заклинание в руку");
    }
}

SpellType Game::getSpellType(const Spell *spell) const {
    return spell ? spell->getType() : SpellType::ENHANCE;
}

Spell *Game::createSpellFromType(SpellType type) {
    SpellFactory factory;
    return factory.createSpell(type);
}

Spell *Game::createSpellFromTypeAndData(std::ifstream &file, SpellType type) {
    SpellFactory factory;
    return factory.createSpell(type, file);
}

void Game::setCellType(int x, int y, CellType type) {
    field.setCell(x, y, type);
}

void Game::clearEnemies() {
    enemies.clear();
}

void Game::addEnemy(int x, int y, int health, int damage) {
    if (health <= 0 || damage <= 0) {
        throw std::invalid_argument("Некорректные параметры врага");
    }
    enemies.emplace_back(x, y, health, damage, field.getWidth(), field.getHeight());
}

void Game::clearBuildings() {
    buildings.clear();
}

void Game::addBuilding(int x, int y, int spawnCooldown, int currentCooldown) {
    if (spawnCooldown <= 0 || currentCooldown < 0) {
        throw std::invalid_argument("Некорректные параметры здания");
    }
    buildings.emplace_back(x, y, spawnCooldown, field.getWidth(), field.getHeight());
    if (currentCooldown != spawnCooldown) {
        buildings.back().resetCooldown();
        for (int i = 0; i < spawnCooldown - currentCooldown; ++i) {
            buildings.back().update();
        }
    }
}

void Game::clearTowers() {
    towers.clear();
}

void Game::addTower(int x, int y, int radius, int cooldown, int currentCooldown) {
    if (radius <= 0 || cooldown <= 0 || currentCooldown < 0) {
        throw std::invalid_argument("Некорректные параметры башни");
    }
    towers.emplace_back(x, y, radius, GameConstants::TOWER_DAMAGE, cooldown, field.getWidth(), field.getHeight());
    while (towers.back().getCurrentCooldown() < currentCooldown && towers.back().getCurrentCooldown() < cooldown) {
        towers.back().update();
    }
}

void Game::clearTraps() {
    traps.clear();
}

void Game::addTrap(int x, int y, int damage, bool active) {
    if (damage <= 0) {
        throw std::invalid_argument("Некорректные параметры ловушки");
    }
    traps.emplace_back(x, y, damage);
    if (!active) {
        traps.back().trigger();
    }
}

void Game::clearAllies() {
    allies.clear();
}

void Game::addAlly(int x, int y, int health, int damage) {
    if (health <= 0 || damage <= 0) {
        throw std::invalid_argument("Некорректные параметры союзника");
    }
    allies.emplace_back(x, y, health, damage, field.getWidth(), field.getHeight());
}

GameSnapshot Game::makeSnapshot() const {
    GameSnapshot snapshot{};
    snapshot.currentLevel = currentLevel;
    snapshot.fieldWidth = field.getWidth();
    snapshot.fieldHeight = field.getHeight();
    snapshot.enemiesKilled = enemiesKilled;

    int playerX, playerY;
    player.getPosition(playerX, playerY);
    snapshot.player = {
        playerX, playerY, player.getHealth(), player.getMeleeDamage(), player.getRangedDamage(),
        player.getScore(), player.getHand().getMaxSize(), player.getBattleType(), player.shouldSkipTurn(), {}
    };

    const Hand &handRef = player.getHand();
    for (int i = 0; i < handRef.getSize(); ++i) {
        Spell *spell = handRef.getSpell(i);
        if (!spell) continue;
        SpellState state;
        state.type = spell->getType();
        std::ostringstream out;
        spell->serialize(out);
        std::istringstream in(out.str());
        int value;
        while (in >> value) {
            state.parameters.push_back(value);
        }
        snapshot.player.hand.push_back(state);
    }

    snapshot.cells.resize(snapshot.fieldHeight, std::vector<CellType>(snapshot.fieldWidth, CellType::EMPTY));
    for (int y = 0; y < snapshot.fieldHeight; ++y) {
        for (int x = 0; x < snapshot.fieldWidth; ++x) {
            snapshot.cells[y][x] = field.getCell(x, y).getType();
        }
    }

    for (const auto &enemy: enemies) {
        int ex, ey;
        enemy.getPosition(ex, ey);
        snapshot.enemies.push_back({ex, ey, enemy.getHealth(), enemy.getDamage()});
    }

    for (const auto &building: buildings) {
        int bx, by;
        building.getPosition(bx, by);
        snapshot.buildings.push_back({bx, by, building.getSpawnCooldown(), building.getCurrentCooldown()});
    }

    for (const auto &tower: towers) {
        int tx, ty;
        tower.getPosition(tx, ty);
        snapshot.towers.push_back({
            tx, ty, tower.getAttackRadius(), tower.getDamage(),
            tower.getCooldown(), tower.getCurrentCooldown()
        });
    }

    for (const auto &trap: traps) {
        snapshot.traps.push_back({trap.getX(), trap.getY(), trap.getDamage(), trap.isActive()});
    }

    for (const auto &ally: allies) {
        int ax, ay;
        ally.getPosition(ax, ay);
        snapshot.allies.push_back({ax, ay, ally.getHealth(), ally.getDamage()});
    }

    snapshot.pendingEnhancements = spellSystem.getPendingEnhancements();
    return snapshot;
}

void Game::applySnapshot(const GameSnapshot &snapshot) {
    if (snapshot.fieldWidth < GameConstants::MIN_FIELD_SIZE || snapshot.fieldWidth > GameConstants::MAX_FIELD_SIZE ||
        snapshot.fieldHeight < GameConstants::MIN_FIELD_SIZE || snapshot.fieldHeight > GameConstants::MAX_FIELD_SIZE) {
        throw std::invalid_argument("Размер поля в сохранении вне допустимых границ");
    }

    Field newField(snapshot.fieldWidth, snapshot.fieldHeight);
    for (int y = 0; y < snapshot.fieldHeight; ++y) {
        if (static_cast<size_t>(y) >= snapshot.cells.size()) {
            throw std::invalid_argument("Высота поля в сохранении не совпадает");
        }
        for (int x = 0; x < snapshot.fieldWidth; ++x) {
            if (static_cast<size_t>(x) >= snapshot.cells[y].size()) {
                throw std::invalid_argument("Ширина поля в сохранении не совпадает");
            }
            newField.setCell(x, y, snapshot.cells[y][x]);
        }
    }

    Player newPlayer(snapshot.player.x, snapshot.player.y, snapshot.player.health,
                     snapshot.fieldWidth, snapshot.fieldHeight, snapshot.player.handCapacity);
    newPlayer.setMeleeDamage(snapshot.player.meleeDamage);
    newPlayer.setRangedDamage(snapshot.player.rangedDamage);
    newPlayer.setScore(snapshot.player.score);
    newPlayer.setBattleType(snapshot.player.battleType);
    newPlayer.setSkipNextTurn(snapshot.player.skipTurn);

    Hand &newHand = newPlayer.getHand();
    SpellFactory factory;
    for (const auto &state: snapshot.player.hand) {
        std::ostringstream out;
        for (size_t i = 0; i < state.parameters.size(); ++i) {
            if (i > 0) out << " ";
            out << state.parameters[i];
        }
        std::istringstream in(out.str());
        Spell *spell = factory.createSpell(state.type, in);
        if (!newHand.addSpell(spell)) {
            delete spell;
            throw std::runtime_error("Не удалось восстановить заклинание в руку");
        }
    }

    std::vector<Enemy> newEnemies;
    for (const auto &st: snapshot.enemies) {
        newEnemies.emplace_back(st.x, st.y, st.health, st.damage, snapshot.fieldWidth, snapshot.fieldHeight);
    }

    std::vector<EnemyBuilding> newBuildings;
    for (const auto &st: snapshot.buildings) {
        newBuildings.emplace_back(st.x, st.y, st.spawnCooldown, snapshot.fieldWidth, snapshot.fieldHeight);
        if (st.currentCooldown != st.spawnCooldown) {
            newBuildings.back().resetCooldown();
            for (int i = 0; i < st.spawnCooldown - st.currentCooldown; ++i) {
                newBuildings.back().update();
            }
        }
    }

    std::vector<EnemyTower> newTowers;
    for (const auto &st: snapshot.towers) {
        newTowers.emplace_back(st.x, st.y, st.radius, st.damage, st.cooldown,
                               snapshot.fieldWidth, snapshot.fieldHeight);
        while (newTowers.back().getCurrentCooldown() < st.currentCooldown &&
               newTowers.back().getCurrentCooldown() < st.cooldown) {
            newTowers.back().update();
        }
    }

    std::vector<Trap> newTraps;
    for (const auto &st: snapshot.traps) {
        Trap trap(st.x, st.y, st.damage);
        if (!st.active) {
            trap.trigger();
        }
        newTraps.push_back(trap);
    }

    std::vector<Ally> newAllies;
    for (const auto &st: snapshot.allies) {
        newAllies.emplace_back(st.x, st.y, st.health, st.damage, snapshot.fieldWidth, snapshot.fieldHeight);
    }

    field = std::move(newField);
    player = std::move(newPlayer);
    enemies = std::move(newEnemies);
    buildings = std::move(newBuildings);
    towers = std::move(newTowers);
    traps = std::move(newTraps);
    allies = std::move(newAllies);
    currentLevel = snapshot.currentLevel;
    enemiesKilled = snapshot.enemiesKilled;
    spellSystem.clearEnhancements();
    for (const auto &enh: snapshot.pendingEnhancements) {
        spellSystem.addEnhancement(enh);
    }
}
