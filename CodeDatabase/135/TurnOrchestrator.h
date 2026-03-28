#ifndef TURN_ORCHESTRATOR_H
#define TURN_ORCHESTRATOR_H

class TurnCoordinator;
class GameRenderer;
class GameField;
class EntityManager;
struct PlayerTurnResult;
struct EnemyTurnResult;
struct AllyTurnResult;
struct TowerTurnResult;
struct UpdateResult;

class TurnOrchestrator {
private:
	TurnCoordinator& turn_coordinator;
	GameRenderer& renderer;
	GameField& field;
	EntityManager& entity_manager;
	int& player_index;
	
	void displayPlayerTurnResult(const PlayerTurnResult& result);
	void displayEnemyTurnResult(const EnemyTurnResult& result);
	void displayAllyTurnResult(const AllyTurnResult& result);
	void displayTowerTurnResult(const TowerTurnResult& result);
	void displayUpdateResult(const UpdateResult& result);

public:
	TurnOrchestrator(TurnCoordinator& tc, GameRenderer& rend, GameField& f, 
	                 EntityManager& em, int& player_idx);
	
	void executeFullTurn();
	void showPlayerTurnStart();
	bool isPlayerAlive() const;
};

#endif
