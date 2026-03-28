#ifndef RPG_ENTITYMANAGER_H
#define RPG_ENTITYMANAGER_H

#include <vector>
#include "Entity.h"
class Player;
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class EntityManager {
    std::shared_ptr<Entity> player;
    std::vector<std::shared_ptr<Entity>> allies;
    std::vector<std::shared_ptr<Entity>> enemies;
    std::vector<std::shared_ptr<Entity>> enemyBuildings;
public:
    EntityManager();

    explicit EntityManager(json& data);

    [[nodiscard]] json to_json() const;

    template <typename T>
    Entity* addEntity(T entity);

    [[nodiscard]] Player* getPlayer() const;

    [[nodiscard]] std::vector<Entity*> getEntities() const;

    [[nodiscard]] std::vector<Entity*> getAIEntities() const;

    [[nodiscard]] Entity* findEntity(const std::pair<int, int>& coordinates) const;

    void processRemovals();

    std::vector<Entity*> getAllies();
};

template <typename T>
Entity* EntityManager::addEntity(T entity) {
    switch (entity.getSymbol()) {
        case 'P':
            player = std::make_shared<T>(std::move(entity));
            return player.get();
        case 'A': case 'M':
            allies.push_back(std::make_shared<T>(std::move(entity)));
            return allies.back().get();
        case 'E':
            enemies.push_back(std::make_shared<T>(std::move(entity)));
            return enemies.back().get();
        case 'T': case 'B':
            enemyBuildings.push_back(std::make_shared<T>(std::move(entity)));
            return enemyBuildings.back().get();
        default:
            break;
    }
    return nullptr;
}

#endif