#ifndef GAME_LOOP_CONTROLLER_H
#define GAME_LOOP_CONTROLLER_H

class GameField;
class EntityManager;
class TurnOrchestrator;
class GameRenderer;
class LevelDirector;
class InputHandler;
class MenuController;
class LevelProgressTracker;

class GameLoopController {
private:
	GameField& field;
	EntityManager& entity_manager;
	int& player_index;
	unsigned int& game_turn;
	
	TurnOrchestrator& turn_orchestrator;
	GameRenderer& renderer;
	LevelDirector& level_director;
	InputHandler& input_handler;
	MenuController& menu_controller;
	LevelProgressTracker& progress_tracker;
	
	bool game_running;

public:
	GameLoopController(GameField& f, EntityManager& em, int& player_idx, unsigned int& turn,
	                   TurnOrchestrator& to, GameRenderer& rend, LevelDirector& ld,
	                   InputHandler& ih, MenuController& mc, LevelProgressTracker& pt);
	
	void runGameLoop();
	void stopGameLoop();
};

#endif
