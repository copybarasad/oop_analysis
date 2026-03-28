#include "BaseTargetedSpell.h"
#include "SpellContext.h"
#include "GameField.h"
#include "EntityManager.h"
#include "SpellEnhancer.h"
#include <cmath>

BaseTargetedSpell::BaseTargetedSpell(int spell_radius) 
	: radius(spell_radius) {}

std::optional<SpellData> BaseTargetedSpell::validateAndExtract(SpellContext& context) {
	int target_x = context.getTargetX();
	int target_y = context.getTargetY();
	int caster_index = context.getCasterIndex();
	GameField& field = context.getField();
	EntityManager& em = context.getEntityManager();
	
	if (!field.isValidPosition(target_x, target_y)) {
		return std::nullopt;
	}
	
	int target_index = field.coordToIndex(target_x, target_y);
	
	int caster_x, caster_y;
	field.indexToCoord(caster_index, caster_x, caster_y);
	
	return SpellData(target_x, target_y, caster_x, caster_y, caster_index, 
	                 target_index, field, em, context.getEnhancer());
}

int BaseTargetedSpell::calculateDistance(int x1, int y1, int x2, int y2) const {
	int dx = x2 - x1;
	int dy = y2 - y1;
	return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}

bool BaseTargetedSpell::cast(SpellContext& context) {
	auto data = validateAndExtract(context);
	if (!data) {
		return false;
	}
	
	return castImpl(*data);
}
