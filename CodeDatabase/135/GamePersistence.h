#ifndef GAME_PERSISTENCE_H
#define GAME_PERSISTENCE_H

#include "GameState.h"
#include "GameSystems.h"
#include "GameStateManager.h"
#include "ScoreSystem.h"
#include "LevelDirector.h"
#include "EntityData.h"
#include <vector>

class GamePersistence {
private:
	GameState& game_state;
	GameSystems& game_systems;
	GameStateManager& state_manager;
	ScoreSystem& score_system;
	LevelDirector& level_director;
	
	Player* getPlayer();
	const Player* getPlayer() const;
	
	std::vector<EntityData> collectAllEntities() const;
	std::vector<int> collectFieldCells() const;
	
	void restoreFieldFromSave(const std::vector<int>& field_cells, int width, int height);
	void restorePlayerFromSave(const GameStateManager::LoadedGameState& state);
	void restoreEntitiesFromSave(const std::vector<EntityData>& entities);

public:
	GamePersistence(GameState& state, GameSystems& systems, 
	                GameStateManager& manager, ScoreSystem& score, 
	                LevelDirector& director);
	
	void saveGame();
	void loadGame();
	bool hasSaveFile() const;
};

#endif







