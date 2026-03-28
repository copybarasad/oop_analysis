#ifndef SPELL_CONTEXT_H
#define SPELL_CONTEXT_H

class GameField;
class EntityManager;
class SpellEnhancer;

class SpellContext {
private:
	GameField& field;
	EntityManager& entity_manager;
	SpellEnhancer& enhancer;
	int caster_index;
	int target_x;
	int target_y;

public:
	SpellContext(GameField& game_field, EntityManager& em, SpellEnhancer& spell_enhancer,
	             int caster_idx, int target_x_coord, int target_y_coord);
	
	GameField& getField();
	const GameField& getField() const;
	
	EntityManager& getEntityManager();
	const EntityManager& getEntityManager() const;
	
	SpellEnhancer& getEnhancer();
	const SpellEnhancer& getEnhancer() const;
	
	int getCasterIndex() const;
	int getTargetX() const;
	int getTargetY() const;
	
	void setTargetX(int x);
	void setTargetY(int y);
};

#endif
