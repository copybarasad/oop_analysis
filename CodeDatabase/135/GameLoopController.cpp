#include "GameLoopController.h"
#include "GameField.h"
#include "EntityManager.h"
#include "TurnOrchestrator.h"
#include "GameRenderer.h"
#include "LevelDirector.h"
#include "InputHandler.h"
#include "MenuController.h"
#include "LevelProgressTracker.h"
#include <iostream>

GameLoopController::GameLoopController(GameField& f, EntityManager& em, int& player_idx, unsigned int& turn,
                                       TurnOrchestrator& to, GameRenderer& rend, LevelDirector& ld,
                                       InputHandler& ih, MenuController& mc, LevelProgressTracker& pt)
	: field(f), entity_manager(em), player_index(player_idx), game_turn(turn),
	  turn_orchestrator(to), renderer(rend), level_director(ld),
	  input_handler(ih), menu_controller(mc), progress_tracker(pt), game_running(true) {}

void GameLoopController::stopGameLoop() {
	game_running = false;
}

void GameLoopController::runGameLoop() {
	renderer.showWelcomeScreen();
	std::cout << "\nНажмите Enter для начала...\n";
	input_handler.clearInputBuffer();
	input_handler.waitForEnter();
	
	while (game_running) {
		
		renderer.render(field, entity_manager, player_index, 
		                level_director.getCurrentLevel(), game_turn);
		
		std::cout << "\n📊 Score: " << progress_tracker.getCurrentScore() 
		          << "/" << progress_tracker.getScoreThreshold() << "\n";
		
		if (!turn_orchestrator.isPlayerAlive()) {
			game_running = menu_controller.handleGameOver(game_turn);
			continue;
		}
		
		UserInput user_input = input_handler.getUserChoice();
		
		if (user_input == UserInput::PAUSE) {
			game_running = menu_controller.handlePauseMenu();
			if (!game_running) {
				break;
			}
			continue;
		}
		
		turn_orchestrator.showPlayerTurnStart();
		turn_orchestrator.executeFullTurn();
		game_turn++;
		
		if (progress_tracker.isLevelComplete()) {
			progress_tracker.handleLevelCompletion();
		}
		
		std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
		input_handler.waitForEnter();
		
		if (!turn_orchestrator.isPlayerAlive()) {
			renderer.render(field, entity_manager, player_index, 
			                level_director.getCurrentLevel(), game_turn);
			game_running = menu_controller.handleGameOver(game_turn);
		}
	}
	
	std::cout << "\n💫 Спасибо за игру!\n";
}
