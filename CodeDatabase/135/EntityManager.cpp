#include "EntityManager.h"
#include "ICombatant.h"

void EntityManager::addEntity(std::unique_ptr<IEntity> entity, int index) {
	entities[index] = std::move(entity);
}

void EntityManager::removeEntity(int index) {
	entities.erase(index);
}

void EntityManager::moveEntity(int from_index, int to_index) {
	if (entities.find(from_index) != entities.end()) {
		entities[to_index] = std::move(entities[from_index]);
		entities.erase(from_index);
	}
}

std::vector<int> EntityManager::findEntitiesByType(EntityType entity_type) const {
	std::vector<int> indexes;
	for (const auto& [index, entity] : entities) {
		if (entity->getType() == entity_type) {
			indexes.push_back(index);
		}
	}
	return indexes;
}

bool EntityManager::hasEntity(int index) const {
	return entities.find(index) != entities.end();
}

std::optional<EntityType> EntityManager::getEntityType(int index) const {
	auto it = entities.find(index);
	if (it != entities.end()) {
		return it->second->getType();
	}
	return std::nullopt;
}

bool EntityManager::isEntityAlive(int index) const {
	auto it = entities.find(index);
	if (it != entities.end()) {
		return it->second->alive();
	}
	return false;
}

void EntityManager::causeDamageToEntity(int index, int damage) {
	auto it = entities.find(index);
	if (it != entities.end()) {
		it->second->causeDamage(damage);
	}
}

std::optional<std::pair<int, int>> EntityManager::getEntityHealth(int index) const {
	auto it = entities.find(index);
	if (it != entities.end()) {
		return it->second->getHealth();
	}
	return std::nullopt;
}

std::optional<int> EntityManager::getEntityDamage(int index) const {
	auto it = entities.find(index);
	if (it != entities.end()) {
		const ICombatant* combatant = dynamic_cast<const ICombatant*>(it->second.get());
		if (combatant) {
			return combatant->getDamage();
		}
	}
	return std::nullopt;
}

IEntity* EntityManager::getEntity(int index) {
	auto it = entities.find(index);
	if (it != entities.end()) {
		return it->second.get();
	}
	return nullptr;
}

const IEntity* EntityManager::getEntity(int index) const {
	auto it = entities.find(index);
	if (it != entities.end()) {
		return it->second.get();
	}
	return nullptr;
}
