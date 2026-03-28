#include "DirectDamageSpell.h"
#include "SpellContext.h"
#include "GameField.h"
#include "EntityManager.h"
#include "SpellEnhancer.h"
#include "IEntity.h"

DirectDamageSpell::DirectDamageSpell(int damage_value, int spell_radius) 
	: BaseTargetedSpell(spell_radius), damage(damage_value) {}

bool DirectDamageSpell::castImpl(SpellData& data) {
	if (!data.entity_manager.hasEntity(data.target_index) || 
	    !data.entity_manager.isEntityAlive(data.target_index)) {
		return false;
	}
	
	auto entity_type = data.entity_manager.getEntityType(data.target_index);
	if (!entity_type.has_value()) {
		return false;
	}
	
	EntityType type = entity_type.value();
	if (type != EntityType::ENEMY && 
	    type != EntityType::BARRACKS && 
	    type != EntityType::TOWER) {
		return false;
	}
	
	int distance = calculateDistance(data.caster_x, data.caster_y, 
	                                 data.target_x, data.target_y);
	
	int enhancement = data.enhancer.getCount();
	int actual_radius = radius + enhancement;
	
	if (distance > actual_radius) {
		return false;
	}
	
	data.enhancer.applyAndReset();
	data.entity_manager.causeDamageToEntity(data.target_index, damage);
	return true;
}
