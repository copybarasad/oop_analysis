#ifndef RPG_ENTITY_MANAGER_H_
#define RPG_ENTITY_MANAGER_H_

#include "Entity.h"
#include <algorithm>
#include <memory>
#include <vector>

template <typename T> class EntityManager {
private:
  std::vector<std::unique_ptr<T>> entities;

public:
  void add(std::unique_ptr<T> entity) { entities.push_back(std::move(entity)); }
  void removeDead();
  std::vector<T *> getAlive() const;
  std::vector<T *> getInRange(const Entity &center, int range) const;
  const auto &getAll() const { return entities; }
  auto &getAll() { return entities; }
  size_t size() const { return entities.size(); }
  bool empty() const { return entities.empty(); }
  void clear() { entities.clear(); }
};

template <typename T> void EntityManager<T>::removeDead() {
  entities.erase(
      std::remove_if(entities.begin(), entities.end(),
                     [](const auto &entity) { return !entity->isAlive(); }),
      entities.end());
}

template <typename T> std::vector<T *> EntityManager<T>::getAlive() const {
  std::vector<T *> alive;
  for (const auto &entity : entities) {
    if (entity && entity->isAlive()) {
      alive.push_back(entity.get());
    }
  }
  return alive;
}

template <typename T>
std::vector<T *> EntityManager<T>::getInRange(const Entity &center,
                                              int range) const {
  std::vector<T *> inRange;
  for (const auto &entity : entities) {
    if (entity && entity->isAlive() && center.distanceTo(*entity) <= range) {
      inRange.push_back(entity.get());
    }
  }
  return inRange;
}

#endif // RPG_ENTITY_MANAGER_H_
