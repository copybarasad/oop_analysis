#include "TrapSpell.h"
#include "SpellContext.h"
#include "GameField.h"
#include "EntityManager.h"
#include "FieldCell.h"
#include "SpellEnhancer.h"
#include "Trap.h"
#include <memory>

TrapSpell::TrapSpell(int damage_value, int spell_radius) 
	: BaseTargetedSpell(spell_radius), damage(damage_value) {}

bool TrapSpell::castImpl(SpellData& data) {
	FieldCell& cell = data.field.getCell(data.target_index);
	if (data.entity_manager.hasEntity(data.target_index) || 
	    cell.hasTrap() || !cell.isPassable()) {
		return false;
	}
	
	int distance = calculateDistance(data.caster_x, data.caster_y, 
	                                 data.target_x, data.target_y);
	
	if (distance > radius) {
		return false;
	}
	
	int enhancement = data.enhancer.getCount();
	int actual_damage = damage + (enhancement * 10);
	
	data.enhancer.applyAndReset();
	
	auto trap = std::make_unique<Trap>(actual_damage);
	cell.setTrap(std::move(trap));
	
	return true;
}
