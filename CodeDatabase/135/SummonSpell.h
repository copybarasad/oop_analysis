#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "ISpell.h"

class GameField;
class EntityManager;

class SummonSpell : public ISpell {
private:
	int ally_count;
	int ally_hp;
	int ally_damage;
	
	bool trySpawnAlly(int x, int y, GameField& field, EntityManager& em, 
	                  int& spawned, int max_count);
	void spawnInRadius(int caster_x, int caster_y, int min_radius, int max_radius,
	                   GameField& field, EntityManager& em, int& spawned, int max_count);

public:
	SummonSpell(int count, int hp, int damage);
	bool cast(SpellContext& context) override;
};

#endif
