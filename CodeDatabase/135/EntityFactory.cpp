#include "EntityFactory.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyBarracks.h"
#include "EnemyTower.h"

EntityFactory::EntityFactory() {
	registerDefaultEntities();
}

void EntityFactory::registerDefaultEntities() {
	registerEntity(EntityType::ENEMY, [](int damage, int max_hp, int) {
		return std::make_unique<Enemy>(damage, max_hp);
	});
	
	registerEntity(EntityType::ALLY, [](int damage, int max_hp, int) {
		return std::make_unique<Ally>(damage, max_hp);
	});
	
	registerEntity(EntityType::BARRACKS, [](int, int max_hp, int spawn_cooldown) {
		return std::make_unique<EnemyBarracks>(max_hp, spawn_cooldown > 0 ? spawn_cooldown : 5);
	});
	
	registerEntity(EntityType::TOWER, [](int damage, int max_hp, int) {
		return std::make_unique<EnemyTower>(max_hp, damage);
	});
}

void EntityFactory::registerEntity(EntityType type, EntityCreator creator) {
	creators[type] = creator;
}

std::unique_ptr<IEntity> EntityFactory::createEntity(EntityType type, int damage, int max_hp, int extra_param) {
	auto it = creators.find(type);
	if (it != creators.end()) {
		return it->second(damage, max_hp, extra_param);
	}
	return nullptr;
}

bool EntityFactory::canCreate(EntityType type) const {
	return creators.find(type) != creators.end();
}
