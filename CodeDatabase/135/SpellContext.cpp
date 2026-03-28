#include "SpellContext.h"

SpellContext::SpellContext(GameField& game_field, EntityManager& em, SpellEnhancer& spell_enhancer,
                           int caster_idx, int target_x_coord, int target_y_coord)
	: field(game_field), 
	  entity_manager(em), 
	  enhancer(spell_enhancer),
	  caster_index(caster_idx), 
	  target_x(target_x_coord), 
	  target_y(target_y_coord) {}

GameField& SpellContext::getField() {
	return field;
}

const GameField& SpellContext::getField() const {
	return field;
}

EntityManager& SpellContext::getEntityManager() {
	return entity_manager;
}

const EntityManager& SpellContext::getEntityManager() const {
	return entity_manager;
}

SpellEnhancer& SpellContext::getEnhancer() {
	return enhancer;
}

const SpellEnhancer& SpellContext::getEnhancer() const {
	return enhancer;
}

int SpellContext::getCasterIndex() const {
	return caster_index;
}

int SpellContext::getTargetX() const {
	return target_x;
}

int SpellContext::getTargetY() const {
	return target_y;
}

void SpellContext::setTargetX(int x) {
	target_x = x;
}

void SpellContext::setTargetY(int y) {
	target_y = y;
}
