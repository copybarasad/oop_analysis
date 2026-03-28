#ifndef GAME_H
#define GAME_H

#include "GameState.h"
#include "GameSystems.h"
#include "GamePersistence.h"
#include "TurnCoordinator.h"
#include "GameRenderer.h"
#include "ScoreSystem.h"
#include "GameMenu.h"
#include "UpgradeSystem.h"
#include "UpgradeMenu.h"
#include "GameStateManager.h"
#include "LevelDirector.h"
#include "TurnOrchestrator.h"
#include "GameRestartHandler.h"
#include "LevelTransitionHandler.h"
#include "MenuController.h"
#include "LevelProgressTracker.h"
#include "GameLoopController.h"
#include "InputHandler.h"
#include <memory>

class Game {
private:
	
	GameSystems game_systems;
	GameState game_state;
	
	ScoreSystem score_system;
	GameStateManager game_state_manager;
	LevelDirector level_director;
	UpgradeSystem upgrade_system;
	
	GamePersistence game_persistence;
	
	TurnCoordinator turn_coordinator;
	TurnOrchestrator turn_orchestrator;
	GameRestartHandler restart_handler;
	LevelTransitionHandler transition_handler;
	
	GameRenderer renderer;
	GameMenu game_menu;
	UpgradeMenu upgrade_menu;
	InputHandler input_handler;
	MenuController menu_controller;
	LevelProgressTracker progress_tracker;
	GameLoopController loop_controller;

public:
	Game();
	void startGameLoop();
	bool showStartScreen();
};

#endif
