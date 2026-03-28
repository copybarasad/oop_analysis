#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <array>
#include <memory>
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include "../../entites/Entity.h"
#include "../../map/Field.h"

template<typename EntityType>
class EntityManager {
public:
    bool SpawnEntities(Field& field, int count, int level);
    bool EntitiesTurn(Field& field, std::shared_ptr<Entity> target = nullptr);
    bool GetMoveForEntity(Field& field, std::array<MoveType, 4>& moves,
                         std::shared_ptr<Entity> target, std::weak_ptr<Entity> current);
    void DeleteDeadEntities(Field& field);
    void AddEntity(std::weak_ptr<EntityType> entity);
    const std::vector<std::weak_ptr<EntityType>>& GetEntities() const;

    std::shared_ptr<Entity> FindTarget(int x, int y, Field& field, bool target_friendly);
    void FindAllEntities(Field& field);

private:
    std::vector<std::weak_ptr<EntityType>> entities_;
};


template<typename EntityType>
bool EntityManager<EntityType>::EntitiesTurn(Field& field, std::shared_ptr<Entity> target) {
    bool flag = true;

    for (auto entity_weak : entities_) {
        if (auto entity = entity_weak.lock()) {
            if (entity->NotMoveNext())
                continue;

            int x_entity, y_entity;
            if (!field.GetPosEntity(entity, x_entity, y_entity)) {
                flag = false;
                continue;
            }

            std::shared_ptr<Entity> actual_target = target;
            if (!actual_target) {
                bool search_friendly = !entity->IsFriendly();
                actual_target = FindTarget(x_entity, y_entity, field, search_friendly);
            }

            if (!actual_target) {
                flag = false;
                continue;
            }

            std::array<MoveType, 4> moving{};
            if (!GetMoveForEntity(field, moving, actual_target, entity_weak)) {
                flag = false;
                continue;
            }

            for (auto move : moving) {
                MoveResult result = field.MoveEntity(move, entity);
                if (result == MoveResult::Moved || result == MoveResult::HitEntity)
                    break;
            }
        }
    }

    return flag;
}

template<typename EntityType>
std::shared_ptr<Entity> EntityManager<EntityType>::FindTarget(int x, int y, Field& field, bool target_friendly) {
    int width, height;
    field.GetSize(width, height);

    std::shared_ptr<Entity> nearest = nullptr;
    double min_dist = std::numeric_limits<double>::max();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            auto entity = field.GetEntity(j, i);
            if (!entity) continue;

            if (entity->IsFriendly() != target_friendly) continue;

            double dist = std::hypot(x - j, y - i);

            if (dist < min_dist) {
                min_dist = dist;
                nearest = entity;
            }
        }
    }

    return nearest;
}

template<typename EntityType>
bool EntityManager<EntityType>::SpawnEntities(Field& field, int count, int level) {
    int w, h;
    field.GetSize(w, h);
    int count_spawn_cells = field.GetCountSpawnCells();

    if (count >= (count_spawn_cells - 1) / 2)
        return false;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(0, w - 1);
    std::uniform_int_distribution<> y_dist(0, h - 1);

    for (int i = 0; i < count; i++) {
        int x = x_dist(gen);
        int y = y_dist(gen);
        auto entity = std::make_shared<EntityType>(20*(level/50));
        entities_.push_back(entity);
        while ((x == 0 && y == 0) || !field.SetEntity(entity, x, y)) {
            x = x_dist(gen);
            y = y_dist(gen);
        }
    }
    return true;
}

template<typename EntityType>
void EntityManager<EntityType>::DeleteDeadEntities(Field& field) {
    for (auto it = entities_.begin(); it != entities_.end();) {
        if (auto entity = it->lock()) {
            if (entity->IsDead()) {
                field.RemoteEntity(entity);
                it = entities_.erase(it);
            } else {
                ++it;
            }
        } else {
            it = entities_.erase(it);
        }
    }
}

template<typename EntityType>
bool EntityManager<EntityType>::GetMoveForEntity(Field& field, std::array<MoveType, 4>& moves,
                                                 std::shared_ptr<Entity> target,
                                                 std::weak_ptr<Entity> current) {
    if (!target) return false;

    int x_target, y_target;
    if (!field.GetPosEntity(target, x_target, y_target))
        return false;

    auto current_shared = current.lock();
    if (!current_shared)
        return false;

    int x_current, y_current;
    if (!field.GetPosEntity(current_shared, x_current, y_current))
        return false;

    int x_maj = x_target - x_current;
    int y_maj = y_target - y_current;

    if (std::abs(x_maj) > std::abs(y_maj)) {
        moves[0] = x_maj > 0 ? MoveType::Right : MoveType::Left;
        moves[2] = x_maj > 0 ? MoveType::Left : MoveType::Right;
        moves[1] = y_maj > 0 ? MoveType::Forward : MoveType::Back;
        moves[3] = y_maj > 0 ? MoveType::Back : MoveType::Forward;
    } else {
        moves[0] = y_maj > 0 ? MoveType::Forward : MoveType::Back;
        moves[2] = y_maj > 0 ? MoveType::Back : MoveType::Forward;
        moves[1] = x_maj > 0 ? MoveType::Right : MoveType::Left;
        moves[3] = x_maj > 0 ? MoveType::Left : MoveType::Right;
    }
    return true;
}

template<typename EntityType>
void EntityManager<EntityType>::AddEntity(std::weak_ptr<EntityType> entity) {
    entities_.push_back(entity);
}

template<typename EntityType>
const std::vector<std::weak_ptr<EntityType>>& EntityManager<EntityType>::GetEntities() const {
    return entities_;
}

template<typename EntityType>
void EntityManager<EntityType>::FindAllEntities(Field& field) {
    entities_.clear();

    int width, height;
    field.GetSize(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto entity = field.GetEntity(x, y);
            if (entity && dynamic_cast<EntityType*>(entity.get())) {
                entities_.push_back(std::dynamic_pointer_cast<EntityType>(entity));
            }
        }
    }
}

#endif // ENTITYMANAGER_H