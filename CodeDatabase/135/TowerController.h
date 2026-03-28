#ifndef TOWER_CONTROLLER_H
#define TOWER_CONTROLLER_H

class GameField;
class EntityManager;
class MovementHelper;

class TowerController {
private:
	GameField& field;
	EntityManager& entity_manager;
	MovementHelper& movement_helper;
	
	int getTowerDamage(int tower_index) const;
	bool tryAttackPlayer(int tower_index, int player_index);
	bool tryAttackAllies(int tower_index);

public:
	TowerController(GameField& game_field, EntityManager& em, MovementHelper& mh);
	void processTowerAttacks(int player_index);
};

#endif
