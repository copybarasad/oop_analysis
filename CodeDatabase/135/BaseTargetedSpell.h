#ifndef BASE_TARGETED_SPELL_H
#define BASE_TARGETED_SPELL_H

#include "ISpell.h"
#include <optional>

class SpellContext;
class GameField;
class EntityManager;
class SpellEnhancer;

struct SpellData {
	int target_x;
	int target_y;
	int caster_x;
	int caster_y;
	int caster_index;
	int target_index;
	GameField& field;
	EntityManager& entity_manager;
	SpellEnhancer& enhancer;
	
	SpellData(int tx, int ty, int cx, int cy, int ci, int ti,
	          GameField& f, EntityManager& em, SpellEnhancer& enh)
		: target_x(tx), target_y(ty), caster_x(cx), caster_y(cy),
		  caster_index(ci), target_index(ti), field(f), 
		  entity_manager(em), enhancer(enh) {}
};

class BaseTargetedSpell : public ISpell {
protected:
	int radius;
	
	std::optional<SpellData> validateAndExtract(SpellContext& context);
	int calculateDistance(int x1, int y1, int x2, int y2) const;
	
	virtual bool castImpl(SpellData& data) = 0;

public:
	explicit BaseTargetedSpell(int spell_radius);
	
	bool cast(SpellContext& context) final override;
};

#endif
