#include "AreaDamageSpell.h"
#include "SpellContext.h"
#include "GameField.h"
#include "EntityManager.h"
#include "SpellEnhancer.h"

AreaDamageSpell::AreaDamageSpell(int damage_value, int spell_radius, int area) 
	: BaseTargetedSpell(spell_radius), damage(damage_value), area_size(area) {}

bool AreaDamageSpell::castImpl(SpellData& data) {
	int distance = calculateDistance(data.caster_x, data.caster_y, 
	                                 data.target_x, data.target_y);
	
	if (distance > radius) {
		return false;
	}
	
	int enhancement = data.enhancer.getCount();
	int actual_area_size = area_size + enhancement;
	
	data.enhancer.applyAndReset();
	
	for (int dy = 0; dy < actual_area_size; ++dy) {
		for (int dx = 0; dx < actual_area_size; ++dx) {
			int x = data.target_x + dx;
			int y = data.target_y + dy;
			
			if (data.field.isValidPosition(x, y)) {
				int index = data.field.coordToIndex(x, y);
				if (data.entity_manager.hasEntity(index) && 
				    data.entity_manager.isEntityAlive(index)) {
					data.entity_manager.causeDamageToEntity(index, damage);
				}
			}
		}
	}
	
	return true;
}
