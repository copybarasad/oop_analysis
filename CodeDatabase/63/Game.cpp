#include "Game.h"
#include "Entity.h"
#include "Player.h"
#include <iostream>
#include <map>
#include <utility>
#include <unistd.h>
#include "IInputManager.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "JsonSaveHandler.h"
#include "JsonLoadHandler.h"
#include "ConsoleRenderer.h"

Game::Game() : level(1), field({{12, 8}, 0.4f, 0.2f}) {
    while (!spawnEntity(Player(100, 20, 30, 5, 10)))
        field.generate();
    spawnEnemies();
}

Game::Game(json& data) :
      field(data.contains("field") ? GameField(data["field"]) : throw std::runtime_error("Отсутствует поле field в сохранении")),
      entityManager(data.contains("entities") ? EntityManager(data["entities"]) : throw std::runtime_error("Отсутствует поле entities в сохранении")) {
    if (!data.contains("level"))
        throw std::runtime_error("Отсутствует поле level в сохранении");
    if (!data["level"].is_number_integer())
        throw std::runtime_error("Значение поля level не является числом");
    level = data["level"].get<int>();
    if (!(1 <= level && level <= 625))
        throw std::runtime_error("Значение поля level принимает некорректное значение");

    for (auto entity: entityManager.getEntities()) {
        if (!field.isInBounds(entity->getPosition()))
            throw std::runtime_error("У одной сущности координаты находятся вне игрового поля.");

        auto target = entity->getTarget();
        auto player = dynamic_cast<Player*>(entity);
        if (target.first == target.second && target.first == -1) {
            if (player && player->getMode() == PlayerModes::RANGE)
                throw std::runtime_error("У игрока не указаны координаты цели, хотя он в режиме дальнего боя.");
        } else {
            if (!field.isInBounds(target))
                throw std::runtime_error("У одной сущности координаты цели находятся вне игрового поля.");
            if (auto spell = entity->getSpellsHand().getSelectedSpell()) {
                if (!field.areCoordinatesInArea(target, entity->getPosition(), spell->getCastDiameter()))
                    throw std::runtime_error("У одной сущности координаты цели находятся вне диаметра примения заклинания.");
            } else if (player) {
                if (!field.areCoordinatesInArea(target, entity->getPosition(), player->getShootRange()))
                    throw std::runtime_error("У игрока координаты цели находятся вне дальности стрельбы.");
            }
        }
        auto& cell = field.getCell(entity->getPosition());
        if (!cell.isPassable())
            throw std::runtime_error("В сохранении сущность оказалась на непроходимой клетке");
        entity->addPoints(cell.entityPlaced());
    }
}

Game& Game::operator=(Game&& other) noexcept {
    if (this == &other) return *this;
    level = other.level;
    field = std::move(other.field);
    entityManager = std::move(other.entityManager);
    return *this;
}

json Game::to_json() {
    json data;
    data["level"] = level;
    data["field"] = field.to_json();
    data["entities"] = entityManager.to_json();
    return data;
}

bool Game::playerAlive() const {
    auto* player = entityManager.getPlayer();
    return player && player->isAlive();
}

void Game::spawnEnemies() {
    for (int i = 1; i <= level; i++)
        spawnEntity(Enemy(40 + 10 * level, 10 + 5 * level, 5));
    spawnEntity(EnemyBuilding(30 + 10 * level, 10 + 5 * level, 9, 7));
    spawnEntity(EnemyTower(30, 20 + 5 * level, 5, 6));
}

void Game::gameCycle() {
    auto* player = entityManager.getPlayer();
    bool stepStatus = false;
    player->reloadSpells();
    auto entities = entityManager.getAIEntities();
    StepRequirements stepRequirements;
    for (auto entity : entities) {
        if (!entity->isAlive()) continue;
        if (entity->isSlowed()) { entity->reloadSpells(); entity->setSlowed(false); continue; }
        stepStatus = false;
        do {
            stepRequirements.enemyPosition = findClosestOpponent(entity, static_cast<int>(entity->getActionDiameter()));
            stepRequirements.wayToEnemy = wayToClosestOpponent(entity, stepRequirements.enemyPosition);
            StepInfo step = entity->move(stepRequirements);
            switch (step.type) {
                case StepType::WALK:
                    stepStatus = moveEntity(step.target, entity);
                    break;
                case StepType::SPELL:
                    stepStatus = castSpell(step.target, entity);
                    break;
                default:
                    break;
            }
        } while (!stepStatus);
        entity->reloadSpells();
        if (!entity->isAlive()) field.getCell(entity->getPosition()).entityDied(entity->getPoints());
    }
    entityManager.processRemovals();
}

States Game::isLevelEnded() {
    auto* player = entityManager.getPlayer();
    if (!player || !player->isAlive())
        return LOSE;

    int aliveEnemies = 0;
    for (auto entity : entityManager.getAIEntities())
        if (entity->getTeam() == EntityTeam::ENEMY && entity->isAlive())
            aliveEnemies++;

    if (aliveEnemies == 0) {
        level++;
        killAllies();
        player->addUpgradePoints(level);
        player->addPoints(field.collectPoints());
        player->setMode(PlayerModes::UPGRADE);
        return WIN;
    }
    return KEEPS;
}

int Game::getLevel() const { return level; }

bool Game::moveEntity(std::pair<int,int>& direction, Entity* entity) {
    if (entity->isSlowed()) {
        entity->setSlowed(false);
        return true;
    }
    auto [dx, dy] = direction;
    if (dx == 0 && dy == 0) return true;
    if (dx == -1 && dy == -1) return false;
    std::pair<int,int> previous = entity->getPosition();
    auto [x, y] = previous;
    std::pair<int,int> next = {x + dx, y + dy};

    if (!field.isInBounds(next)) return false;
    auto& cell = field.getCell(next);
    if (cell.getPassability() == CellType::WALL) return false;
    if (cell.isOccupied()) {
        auto occupant = entityManager.findEntity(next);
        if (occupant->getTeam() == entity->getTeam()) return false;
        occupant->onEnter(entity);
        if (occupant->takeDamage(entity->getDamage())) {
            cell.entityDied(occupant->getPoints());
        } else return true;
    }

    field.getCell(previous).entityLeaved();
    entity->setPosition(next);
    entity->addPoints(cell.entityPlaced());
    if (cell.getPassability() == CellType::SLOW) entity->setSlowed(true);
    return true;
}

bool Game::checkAimBorders(std::pair<int,int>& direction, Entity* entity) const {
    auto [dx, dy] = direction;
    auto [tx, ty] = entity->getTarget();
    auto player = dynamic_cast<Player*>(entity);
    if (!player) return false;
    if (!field.areCoordinatesInArea({tx + dx, ty + dy}, entity->getPosition(), player->getShootRange())) return false;
    entity->setTarget({tx + dx, ty + dy});
    return true;
}

bool Game::shootEntity(const std::pair<int,int>& targetPosition, Entity* shooter) {
    auto player = dynamic_cast<Player*>(shooter);
    if (!player) return false;
    auto* target = entityManager.findEntity(targetPosition);
    if (!target) return true;
    if (target->getTeam() == shooter->getTeam()) return false;
    if (target->takeDamage(shooter->getDamage())) field.getCell(targetPosition).entityDied(target->getPoints());
    return true;
}

bool Game::checkAimSpellBorders(std::pair<int,int>& direction, Entity* entity) const {
    auto [dx, dy] = direction;
    auto [tx, ty] = entity->getTarget();
    ISpell* spell = entity->getSpellsHand().getSelectedSpell();
    if (!spell) return false;
    if (!field.areCoordinatesInArea({tx + dx, ty + dy}, entity->getPosition(), spell->getCastDiameter())) return false;
    entity->setTarget({tx + dx, ty + dy});
    return true;
}

bool Game::castSpell(std::pair<int,int>& targetPosition, Entity* caster) {
    ISpell* spell = caster->getSpellsHand().getSelectedSpell();
    if (!spell) return false;
    return spell->cast(targetPosition, caster, field, entityManager);
}

std::pair<int,int> Game::findClosestOpponent(const Entity* to, int diameter) const {
    if (diameter <= 0) return {-1, -1};
    auto [x, y] = to->getPosition();
    std::pair<int, int> coordinates = {-1, -1};
    int bestDist = diameter * diameter;
    for (auto entity : entityManager.getEntities()) {
        if (!entity->isAlive() || entity->getTeam() == to->getTeam()) continue;
        auto entityPosition = entity->getPosition();
        auto [tx, ty] = entity->getPosition();
        int dist = std::max(std::abs(tx - x), std::abs(ty - y)) / 2;
        if (field.areCoordinatesInArea(entityPosition, to->getPosition(), diameter) && dist < bestDist) {
            bestDist = dist;
            coordinates = entity->getPosition();
        }
    }
    return coordinates;
}

std::pair<int,int> Game::wayToClosestOpponent(const Entity* to, std::pair<int,int> enemyPosition) {
    auto [x, y] = to->getPosition();
    std::vector<std::pair<int,int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    if (enemyPosition.first == -1) {
        std::ranges::shuffle(directions.begin(), directions.end(), field.getRandomGenerator());
        for (auto direction : directions) {
            auto [dx, dy] = direction;
            std::pair<int,int> nextPosition = {x + dx, y + dy};
            auto& cell = field.getCell(nextPosition);
            if (!field.isInBounds(nextPosition) || cell.isOccupied() || cell.getPassability() == CellType::WALL) continue;
            return direction;
        }
        return SUCCESSFUL_STEP;
    }
    auto [tx, ty] = enemyPosition;
    std::queue<std::pair<int,int>> q;
    std::map<std::pair<int,int>, std::pair<int,int>> parent;
    q.emplace(x, y);
    parent[{x, y}] = {-1, -1};
    int maxDepth = field.getWidth() * field.getHeight();
    int depth = 0;
    while (!q.empty() && depth < maxDepth) {
        auto [sx, sy] = q.front(); q.pop();
        depth++;
        if (sx == tx && sy == ty) break;
        for (auto [dx, dy] : directions) {
            int nx = sx + dx, ny = sy + dy;
            std::pair<int,int> pos = {nx, ny};
            if (!field.isInBounds(pos)) continue;
            if (parent.contains(pos)) continue;
            Cell& cell = field.getCell(pos);
            if (cell.getPassability() == CellType::WALL) continue;
            if (cell.isOccupied() && entityManager.findEntity(pos)->getTeam() == to->getTeam()) continue;
            parent[pos] = {sx, sy}; q.push(pos);
        }
    }
    if (!parent.contains({tx, ty})) return SUCCESSFUL_STEP;
    std::pair<int,int> current = {tx, ty};
    while (parent[current] != std::pair<int,int>{x, y} && parent[current] != std::pair<int,int>{-1,-1}) current = parent[current];
    return {current.first - x, current.second - y};
}

void Game::killAllies() {
    for (auto ally : entityManager.getAllies()) {
        field.getCell(ally->getPosition()).entityDied(ally->getPoints());
        ally->takeDamage(ally->getHealth());
    }
    entityManager.processRemovals();
}

bool Game::nextLevel() {
    auto player = entityManager.getPlayer();
    player->upgrade(field.getRandomGenerator());
    field.expand();
    auto playerPosition = field.getFreeCellCoordinates();
    while (playerPosition.first == -1) {
        field.generate();
        playerPosition = field.getFreeCellCoordinates();
    }
    player->setPosition(playerPosition);
    player->addPoints(field.getCell(playerPosition).entityPlaced());
    player->restoreHealth();
    player->setMode(PlayerModes::MELEE);
    spawnEnemies();
    return false;
}

Player& Game::getPlayer() { return *entityManager.getPlayer(); }

GameField& Game::getGameField() { return field; }

EntityManager& Game::getEntityManager() { return entityManager; }

Shop& Game::getShop() { return shop; }
