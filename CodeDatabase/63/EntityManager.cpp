#include "EntityManager.h"
#include "Ally.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Player.h"
#include "Trap.h"

EntityManager::EntityManager() : player(nullptr) {}

EntityManager::EntityManager(json &data) {
    for (auto entity : data) {
        if (!entity.contains("symbol"))
            throw std::runtime_error("У сущности отсутствует поле symbol");
        if (!entity["symbol"].is_number_integer())
            throw std::runtime_error("Значение поля symbol у сущности не является символом");
        switch (entity["symbol"].get<char>()) {
            case 'P':
                player = std::make_shared<Player>(entity);
                break;
            case 'A':
                allies.push_back(std::make_shared<Ally>(entity));
                break;
            case 'M':
                allies.push_back(std::make_shared<Trap>(entity));
                break;
            case 'E':
                enemies.push_back(std::make_shared<Enemy>(entity));
                break;
            case 'T':
                enemyBuildings.push_back(std::make_shared<EnemyTower>(entity));
                break;
            case 'B':
                enemyBuildings.push_back(std::make_shared<EnemyBuilding>(entity));
                break;
            default:
                throw std::runtime_error("Поле symbol у сущности принимает недействительное значение");
        }
    }
}

json EntityManager::to_json() const {
    // json j;
    std::vector<json> entities;
    entities.reserve(1 + allies.size() + enemies.size() + enemyBuildings.size());
    for (auto entity: getEntities())
        entities.push_back(entity->to_json());
    // j = entities;
    return entities;
}

Player* EntityManager::getPlayer() const { return dynamic_cast<Player*>(player.get()); }

std::vector<Entity*> EntityManager::getEntities() const {
    std::vector<Entity*> raw;
    raw.reserve((player != nullptr) + allies.size() + enemies.size() + enemyBuildings.size());
    if (player != nullptr) { raw.push_back(player.get()); }
    for (auto& e : allies)
        raw.push_back(e.get());
    for (auto& e : enemies)
        raw.push_back(e.get());
    for (auto& e : enemyBuildings)
        raw.push_back(e.get());
    return raw;
}

std::vector<Entity*> EntityManager::getAIEntities() const {
    std::vector<Entity*> raw;
    raw.reserve(allies.size() + enemies.size() + enemyBuildings.size());
    for (auto& e : allies)
        raw.push_back(e.get());
    for (auto& e : enemies)
        raw.push_back(e.get());
    for (auto& e : enemyBuildings)
        raw.push_back(e.get());
    return raw;
}

Entity* EntityManager::findEntity(const std::pair<int,int>& coordinates) const {
    auto [x, y] = coordinates;
    for (auto entity : getEntities()) {
        auto [ex, ey] = entity->getPosition();
        if (entity->isAlive() && x == ex && y == ey) {
            return entity;
        }
    }
    return nullptr;
}

void EntityManager::processRemovals() {
    if (!player->isAlive()) player = nullptr;

    auto it = allies.begin();
    while (it != allies.end()) {
        if (!(*it)->isAlive()) {
            it = allies.erase(it);
        } else {
            ++it;
        }
    }

    it = enemies.begin();
    while (it != enemies.end()) {
        if (!(*it)->isAlive()) {
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    it = enemyBuildings.begin();
    while (it != enemyBuildings.end()) {
        if (!(*it)->isAlive()) {
            it = enemyBuildings.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<Entity*> EntityManager::getAllies() {
    std::vector<Entity*> entities;
    entities.reserve(allies.size());
    for (auto& ally : allies)
        entities.push_back(ally.get());
    return entities;
}
