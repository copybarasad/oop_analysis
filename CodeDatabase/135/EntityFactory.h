#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include "IEntity.h"
#include <memory>
#include <functional>
#include <unordered_map>

class EntityFactory {
public:
	using EntityCreator = std::function<std::unique_ptr<IEntity>(int, int, int)>;
	
private:
	std::unordered_map<EntityType, EntityCreator> creators;
	
	void registerDefaultEntities();

public:
	EntityFactory();
	
	void registerEntity(EntityType type, EntityCreator creator);
	
	std::unique_ptr<IEntity> createEntity(EntityType type, int damage, int max_hp, int extra_param = 0);
	
	bool canCreate(EntityType type) const;
};

#endif
