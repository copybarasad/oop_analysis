#include "Game.h"
#include "Player.h"
#include "GameException.h"
#include <iostream>
#include <limits>

Game::Game() 
	: game_systems(),
	  game_state(15, 15, game_systems.getRandomGenerator()),
	  score_system(),
	  game_state_manager(),
	  level_director(game_systems.getRandomGenerator(), game_systems.getSpellFactory(), game_systems.getEntityFactory()),
	  upgrade_system(),
	  game_persistence(game_state, game_systems, game_state_manager, score_system, level_director),
	  turn_coordinator(game_state.getField(), game_state.getEntityManager(), 
	                   game_state.getPlayerIndexRef(), 
	                   game_systems.getSpellFactory(), game_systems.getRandomGenerator(), score_system),
	  turn_orchestrator(turn_coordinator, renderer, game_state.getField(), game_state.getEntityManager(), 
	                    game_state.getPlayerIndexRef()),
	  restart_handler(game_state.getField(), game_state.getEntityManager(), 
	                  game_state.getPlayerIndexRef(), level_director, score_system, game_systems.getRandomGenerator()),
	  transition_handler(game_state.getField(), game_state.getEntityManager(), 
	                     game_state.getPlayerIndexRef(), level_director, score_system, 
	                     renderer, upgrade_system, upgrade_menu, game_systems.getSpellFactory(), game_systems.getRandomGenerator()),
	  renderer(),
	  game_menu(),
	  upgrade_menu(),
	  input_handler(),
	  menu_controller(game_menu, renderer, restart_handler, input_handler),
	  progress_tracker(score_system, transition_handler),
	  loop_controller(game_state.getField(), game_state.getEntityManager(), 
	                  game_state.getPlayerIndexRef(), game_state.getGameTurnRef(),
	                  turn_orchestrator, renderer, level_director, input_handler,
	                  menu_controller, progress_tracker) {
	
	auto init_result = level_director.initializeLevel(1);
	game_state.getField() = std::move(init_result.field);
	game_state.getEntityManager() = std::move(init_result.entity_manager);
	game_state.setPlayerIndex(init_result.player_index);
	score_system.setThreshold(init_result.score_threshold);
	
	menu_controller.setSaveGameCallback([this]() {
		try {
			game_persistence.saveGame();
			std::cout << "\n✓ Игра успешно сохранена!\n";
		} catch (const GameException& e) {
			std::cout << "\n✗ Ошибка сохранения игры: " << e.what() << "\n";
		}
		input_handler.waitForEnter();
	});
	
	menu_controller.setLoadGameCallback([this]() {
		try {
			game_persistence.loadGame();
			std::cout << "\n✓ Игра успешно загружена!\n";
		} catch (const GameException& e) {
			std::cout << "\n✗ Ошибка загрузки игры: " << e.what() << "\n";
		}
		input_handler.waitForEnter();
	});
}

bool Game::showStartScreen() {
	MenuAction action = game_menu.showStartMenu();
	
	switch (action) {
		case MenuAction::START_NEW_GAME:
			return true;
		case MenuAction::LOAD_GAME:
			try {
				game_persistence.loadGame();
				std::cout << "\n✓ Игра успешно загружена!\n";
				input_handler.waitForEnter();
				return true;
			} catch (const GameException& e) {
				std::cout << "\n✗ Ошибка загрузки игры: " << e.what() << "\n";
				std::cout << "Начинаем новую игру...\n";
				input_handler.waitForEnter();
				return true;
			}
		case MenuAction::EXIT_GAME:
			return false;
		default:
			return true;
	}
}

void Game::startGameLoop() {
	loop_controller.runGameLoop();
}
