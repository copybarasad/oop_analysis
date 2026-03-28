#ifndef TRAP_MANAGER_H
#define TRAP_MANAGER_H

class GameField;
class EntityManager;

class TrapManager {
private:
	GameField& field;
	EntityManager& entity_manager;

public:
	TrapManager(GameField& game_field, EntityManager& em);
	void checkTraps(int entity_index);
};

#endif
