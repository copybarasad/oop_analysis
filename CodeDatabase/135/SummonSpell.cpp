#include "SummonSpell.h"
#include "SpellContext.h"
#include "GameField.h"
#include "EntityManager.h"
#include "Ally.h"
#include "FieldCell.h"
#include "GameConstants.h"
#include "SpellEnhancer.h"
#include <memory>

SummonSpell::SummonSpell(int count, int hp, int damage) 
	: ally_count(count), ally_hp(hp), ally_damage(damage) {}

bool SummonSpell::trySpawnAlly(int x, int y, GameField& field, EntityManager& em, int& spawned, int max_count) {
	if (spawned >= max_count) {
		return false;
	}
	
	if (!field.isValidPosition(x, y)) {
		return false;
	}
	
	int index = field.coordToIndex(x, y);
	const FieldCell& cell = field.getCell(index);
	
	if (cell.isPassable() && !em.hasEntity(index)) {
		auto ally = std::make_unique<Ally>(ally_damage, ally_hp);
		em.addEntity(std::move(ally), index);
		++spawned;
		return true;
	}
	
	return false;
}

void SummonSpell::spawnInRadius(int caster_x, int caster_y, int min_radius, int max_radius,
                                 GameField& field, EntityManager& em, int& spawned, int max_count) {
	for (int dy = -max_radius; dy <= max_radius && spawned < max_count; ++dy) {
		for (int dx = -max_radius; dx <= max_radius && spawned < max_count; ++dx) {
			
			int abs_dx = dx < 0 ? -dx : dx;
			int abs_dy = dy < 0 ? -dy : dy;
			
			if (abs_dx <= min_radius && abs_dy <= min_radius) {
				if (dx == 0 && dy == 0) continue;
				if (min_radius > 0) continue;
			}
			
			trySpawnAlly(caster_x + dx, caster_y + dy, field, em, spawned, max_count);
		}
	}
}

bool SummonSpell::cast(SpellContext& context) {
	int caster_index = context.getCasterIndex();
	GameField& field = context.getField();
	EntityManager& em = context.getEntityManager();
	
	int caster_x, caster_y;
	field.indexToCoord(caster_index, caster_x, caster_y);
	
	int enhancement = context.getEnhancer().getCount();
	int actual_count = ally_count + enhancement;
	
	context.getEnhancer().applyAndReset();
	
	int spawned = 0;
	
	spawnInRadius(caster_x, caster_y, 0, 1, field, em, spawned, actual_count);
	
	if (spawned < actual_count) {
		spawnInRadius(caster_x, caster_y, 1, 2, field, em, spawned, actual_count);
	}
	
	return true;
}
