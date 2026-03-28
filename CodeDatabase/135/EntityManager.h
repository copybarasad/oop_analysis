#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "IEntity.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>

class EntityManager {
private:
	std::unordered_map<int, std::unique_ptr<IEntity>> entities;

public:
	EntityManager() = default;
	
	EntityManager(const EntityManager& other) = delete;
	EntityManager& operator=(const EntityManager& other) = delete;
	
	EntityManager(EntityManager&& other) noexcept = default;
	EntityManager& operator=(EntityManager&& other) noexcept = default;
	
	void addEntity(std::unique_ptr<IEntity> entity, int index);
	void removeEntity(int index);
	void moveEntity(int from_index, int to_index);
	std::vector<int> findEntitiesByType(EntityType entity_type) const;
	
	bool hasEntity(int index) const;
	std::optional<EntityType> getEntityType(int index) const;
	bool isEntityAlive(int index) const;
	
	void causeDamageToEntity(int index, int damage);
	std::optional<std::pair<int, int>> getEntityHealth(int index) const;
	std::optional<int> getEntityDamage(int index) const;
	
	IEntity* getEntity(int index);
	const IEntity* getEntity(int index) const;
};

#endif
