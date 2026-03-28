#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>
#include "../../gamecore/include/Enums.h"
#include "Entity.h"

class EntityRegistry {
public:
    EntityRegistry() = default;

    EntityId create(std::unique_ptr<Entity> e);

    void destroy(EntityId id);

    Entity* get(EntityId id);
    const Entity* get(EntityId id) const;

    bool alive(EntityId id) const;

    size_t size() const;

    // Для загрузки: вставить сущность с конкретным ID
    void restore(EntityId id, std::unique_ptr<Entity> e);
    
    void saveState(std::ostream& os) const;
    void loadState(std::istream& is);

    // Итератор для сохранения (или доступ к storage)
    const std::unordered_map<EntityId, std::unique_ptr<Entity>>& storage() const { return storage_; }

private:
    std::unordered_map<EntityId, std::unique_ptr<Entity>> storage_;
    std::vector<EntityId> free_;
    EntityId nextId_{0};
};
