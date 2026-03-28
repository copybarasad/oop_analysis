#include "Game.hpp"
#include "DirectDamageSpell.hpp"
#include "AreaDamageSpell.hpp"
#include "TrapSpell.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <unistd.h>

Game::Game()
	: stateManager(),
	  field(nullptr),
	  world(nullptr),
	  player(100, 15) {
}

void Game::run() {
	while (stateManager.getState() != GameState::Exiting) {
		try {
			switch (stateManager.getState()) {
				case GameState::Menu:
					gameMenu();
					break;
				case GameState::Playing:
					gameLoop();
					break;
				case GameState::LevelComplete:
					handleLevelComplete();
					break;
				case GameState::GameOver:
					handleGameOver();
					break;
				case GameState::Exiting:
					break;
			}
		} catch (const GameException& e) {
			std::cerr << "|| ошибка: " << e.what();
			if (!e.getLocation().empty()) {
				std::cerr << " (в " << e.getLocation() << ")";
			}
			std::cerr << "\n";
			std::cerr << "|| Игра продолжается...\n";
		} catch (const std::exception& e) {
			std::cerr << "|| критическая ошибка: " << e.what() << "\n";
			std::cerr << "|| игра продолжается...\n";
		}
	}
}

void Game::gameMenu() {
	GameMenu::printMenu(saveFileExists("savegame.dat"));
	
	char choice = GameMenu::getMenuChoice();
	
	switch (choice) {
		case '1':
			startNewGame();
			break;
		case '2': {
			const std::string defaultFilename = "savegame.dat";
			try {
				loadGame(defaultFilename);
			} catch (const GameException& e) {
				std::cout << "|| " << e.what() << "\n";
			}
			break;
		}
		case '3':
			stateManager.setState(GameState::Exiting);
			std::cout << "|| До свидания!\n";
			break;
		default:
			std::cout << "|| Неверно. Попробуйте снова.\n";
			break;
	}
}

void Game::startNewGame() {
	stateManager.setCurrentLevel(1);
	stateManager.setKillsRequired(LevelManager::calculateKillsRequired(1));
	stateManager.resetKillsForReward();
	player = Player(100, 15);
	startLevel(1);
}

void Game::startLevel(int level) {
	stateManager.setCurrentLevel(level);
	stateManager.setKillsRequired(LevelManager::calculateKillsRequired(level));
	stateManager.resetKillsForReward();
	
	if (level > 1) {
		player.setHP(player.getMaxHP());
	}
	
	field = std::make_unique<Field>(LevelManager::FIELD_WIDTH, LevelManager::FIELD_HEIGHT);
	world = std::make_unique<WorldManager>(*field);
	
	LevelManager::initializeLevel(level, *field, *world);
	
	world->addPlayer(Coords{0, field->getHeight() - 1});
	
	stateManager.setState(GameState::Playing);
	clearScreen();
	std::cout << "\n";
	std::cout << "   переход на новый уровень!\n";
	std::cout << "уровень " << level << " начат!                    \n";
	std::cout << "убейте " << stateManager.getKillsRequired() << " врагов для прохождения  \n";
	std::cin.ignore();
	std::cin.get();
}

void Game::gameLoop() {
	while (stateManager.getState() == GameState::Playing) {
		clearScreen();
		printGameState();
		
		if (!player.isAlive()) {
			stateManager.setState(GameState::GameOver);
			break;
		}
		
		if (isLevelComplete()) {
			stateManager.setState(GameState::LevelComplete);
			break;
		}
		
		processPlayerTurn();
		
		if (stateManager.getState() != GameState::Playing) break;
		
		checkKillRewards();
		
		processEnemiesTurn();
		checkKillRewards();
		
		processBuildingsTurn();
		checkKillRewards();
		
		processTowersTurn();
		checkKillRewards();
		
		checkGameState();
	}
}

void Game::clearScreen() const {
	#ifdef _WIN32
		(void)system("cls");
	#else
		(void)system("clear");
	#endif
}

void Game::printGameState() const {
	std::vector<Coords> trapPositions;
	const auto& traps = world->getSpellManager().getTraps();
	for (const auto& trap : traps) {
		if (trap.isActive()) {
			trapPositions.push_back(trap.getPosition());
		}
	}
	field->print(player, world->getKillCount(), &trapPositions);
	world->getSpellManager().printHand();
	std::cout << "|| Уровень: " << stateManager.getCurrentLevel() << " | Убийств: " << world->getKillCount() 
			  << "/" << stateManager.getKillsRequired() << "\n";
}

void Game::checkKillRewards() {
	int currentKills = world->getKillCount();
	int newRewards = (currentKills - stateManager.getKillsForReward()) / GameStateManager::KILLS_FOR_NEW_SPELL;
	if (newRewards > 0) {
		for (int i = 0; i < newRewards; ++i) {
			if (world->getSpellManager().addRandomSpell()) {
				std::cout << "|| Вы получили новое заклинание!\n";
			}
		}
		stateManager.setKillsForReward(stateManager.getKillsForReward() + newRewards * GameStateManager::KILLS_FOR_NEW_SPELL);
	}
}

void Game::processPlayerTurn() {
	GameMenu::printCommands();
	
	char input;
	std::cin >> input;
	
	if (input == 'g') {
		const std::string defaultFilename = "savegame.dat";
		try {
			saveGame(defaultFilename);
			std::cout << "|| Игра сохранена в " << defaultFilename << "\n";
			std::cin.ignore();
			std::cin.get();
		} catch (const GameException& e) {
			std::cout << "|| " << e.what() << "\n";
			std::cin.ignore();
			std::cin.get();
		}
		return;
	}
	
	if (input == 'l') {
		const std::string defaultFilename = "savegame.dat";
		try {
			loadGame(defaultFilename);
		} catch (const GameException& e) {
			std::cout << "|| " << e.what() << "\n";
			std::cin.ignore();
			std::cin.get();
		}
		return;
	}
	
	if (input == 'q') {
		stateManager.setState(GameState::Exiting);
		std::cout << "|| Выход из игры\n";
		return;
	}
	
	CommandHandler::handleCommand(input, *world, player);
}

void Game::processEnemiesTurn() {
	world->moveEnemies(player);
}

void Game::processBuildingsTurn() {
	world->updateBuildings();
}

void Game::processTowersTurn() {
	world->updateTowers(player);
}

bool Game::isLevelComplete() const {
	return world->getKillCount() >= stateManager.getKillsRequired();
}

bool Game::isGameWon() const {
	return stateManager.getCurrentLevel() >= GameStateManager::MAX_LEVEL && isLevelComplete();
}

void Game::checkGameState() {
	if (!player.isAlive()) {
		stateManager.setState(GameState::GameOver);
	} else if (isLevelComplete()) {
		stateManager.setState(GameState::LevelComplete);
	}
}

void Game::handleLevelComplete() {
	clearScreen();
	std::cout << "\n";
	std::cout << "         уровень пройден                 \n";
	std::cout << "   уровень " << stateManager.getCurrentLevel() << " завершен!\n";
	std::cout << "   убийств: " << world->getKillCount() << "\n";
	std::cin.ignore();
	std::cin.get();
	
	if (isGameWon()) {
		clearScreen();
		std::cout << "\n";
		std::cout << "         игра завершена!\n";
		std::cin.get();
		stateManager.setState(GameState::Menu);
	} else {
		int nextLevel = stateManager.getCurrentLevel() + 1;
		startLevel(nextLevel);
	}
}

void Game::handleGameOver() {
	std::cout << "|| игрок погиб! конец игры.\n";
	std::cout << "|| достигнуто убийств: " << world->getKillCount() << "\n";
	std::cout << "|| пройдено уровней: " << (stateManager.getCurrentLevel() - 1) << "\n";
	
	std::cout << "|| 1. Начать заново\n";
	std::cout << "|| 2. Выход\n";
	std::cout << "|| Выберите действие: ";
	
	char choice;
	std::cin >> choice;
	
	if (choice == '1') {
		startNewGame();
	} else {
		stateManager.setState(GameState::Exiting);
	}
}

bool Game::saveFileExists(const std::string& filename) const {
	std::ifstream file(filename);
	return file.good();
}

void Game::saveGame(const std::string& filename) {
	try {
		SaveManager save(filename);
		GameSerializer::serializeGame(save, stateManager, player, *field, *world);
	} catch (const FileException& e) {
		throw SaveException("Не удалось создать файл сохранения: " + std::string(e.what()), "Game::saveGame");
	} catch (const std::exception& e) {
		throw SaveException("Ошибка при сохранении: " + std::string(e.what()), "Game::saveGame");
	}
}

void Game::loadGame(const std::string& filename) {
	try {
		if (!saveFileExists(filename)) {
			throw FileException("Файл сохранения не существует: " + filename, "Game::loadGame");
		}
		
		SaveManager load(filename, true);
		GameSerializer::deserializeGame(load, stateManager, player, field, world);
		
		stateManager.setState(GameState::Playing);
		
		clearScreen();
		std::cout << "\n";
		std::cout << "         игра загружена!                 \n";
		std::cout << "  уровень: " << stateManager.getCurrentLevel() << "\n";
		std::cout << "  убийств: " << world->getKillCount() << "/" << stateManager.getKillsRequired() << "\n";
		std::cout << "  HP: " << player.getHP() << "/" << player.getMaxHP() << "\n";
		std::cin.ignore();
		std::cin.get();
	} catch (const FileException& e) {
		throw LoadException("Не удалось открыть файл сохранения: " + std::string(e.what()), "Game::loadGame");
	} catch (const DataException& e) {
		throw LoadException("Файл сохранения поврежден: " + std::string(e.what()), "Game::loadGame");
	} catch (const std::exception& e) {
		throw LoadException("Ошибка при загрузке: " + std::string(e.what()), "Game::loadGame");
	}
}


