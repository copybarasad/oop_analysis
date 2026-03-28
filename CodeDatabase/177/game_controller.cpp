#include "game_controller.h"
#include "spell_card.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <memory>

GameController::GameController(int fieldWidth, int fieldHeight, bool autoInitialize)
	: field(fieldWidth, fieldHeight), gameRunning(true), currentLevel(1), gameState(GameState::PLAYING) {
	
	enemyManager = std::make_unique<EnemyManager>(field);
	buildingManager = std::make_unique<BuildingManager>(field);
	combatManager = std::make_unique<CombatManager>(field);
	playerController = std::make_unique<PlayerController>(player, field);
	gameInitializer = std::make_unique<GameInitializer>(player, field, *enemyManager, *buildingManager, towers);
	turnProcessor = std::make_unique<TurnProcessor>(player, *enemyManager, *buildingManager, *combatManager, gameRunning, towers);
	
	if (autoInitialize) {
		initializeLevel(currentLevel);
	}
}

void GameController::initializeLevel(int level) {
	LevelConfig config = getLevelConfig(level);
	
	std::cout << "\n" << std::string(50, '=') << std::endl;
	std::cout << "STARTING LEVEL " << level << std::endl;
	std::cout << std::string(50, '=') << std::endl;
	std::cout << "Field Size: " << config.fieldWidth << "x" << config.fieldHeight << std::endl;
	std::cout << "Enemies: " << config.enemyCount << " (HP: " << config.enemyHealth << ", DMG: " << config.enemyDamage << ")" << std::endl;
	std::cout << "Buildings: " << config.buildingCount << std::endl;
	
	field = GameField(config.fieldWidth, config.fieldHeight);
	
	if (level == 1) player = Player(config.playerHealth);
	else player.heal(config.playerHealth - player.getHealth());
	
	player.setMana(player.getMaxMana());
	
	if (level > 1) {
		std::cout << "Removing half of spell cards...\n";
		removeHalfOfSpells();
	}
	
	enemyManager = std::make_unique<EnemyManager>(field);
	enemyManager->setEnemyParams(config.enemyHealth, config.enemyDamage);
	buildingManager = std::make_unique<BuildingManager>(field);
	combatManager = std::make_unique<CombatManager>(field);
	playerController = std::make_unique<PlayerController>(player, field);
	gameInitializer = std::make_unique<GameInitializer>(player, field, *enemyManager, *buildingManager, towers);
	turnProcessor = std::make_unique<TurnProcessor>(player, *enemyManager, *buildingManager, *combatManager, gameRunning, towers);
	gameInitializer->initializeGameWithConfig(config.enemyCount, config.buildingCount, config.enemyHealth, config.enemyDamage);
	
	gameState = GameState::PLAYING;
	gameRunning = true;
}

void GameController::movePlayer(Direction direction) {
	if (!isGameRunning() || gameState != GameState::PLAYING) return;
	
	playerController->movePlayer(direction);
	turnProcessor->processGameTurn(); 
	checkLevelCompletion();
	if (!player.isAlive()) {
		gameState = GameState::GAME_OVER;
		gameRunning = false;
	}
}

void GameController::switchPlayerCombatMode() {
	if (!isGameRunning() || gameState != GameState::PLAYING) return;
	playerController->switchCombatMode();
	turnProcessor->processCombatModeSwitch(); 
}

void GameController::castSpell(int spellIndex, const Position& target) {
	if (!isGameRunning() || gameState != GameState::PLAYING) return;
	
	combatManager->processSpellCast(player, spellIndex, target, 
		enemyManager->getEnemies(), buildingManager->getBuildings(), towers);
	
	turnProcessor->processGameTurn(); 
	checkLevelCompletion();
}

const std::vector<Enemy>& GameController::getEnemies() const {
	return enemyManager->getEnemies();
}

const std::vector<EnemyBuilding>& GameController::getBuildings() const {
	return buildingManager->getBuildings();
}

const std::vector<Position>& GameController::getActiveTraps() const {
	return combatManager->getActiveTraps();
}

void GameController::checkLevelCompletion() {
	if (player.getScore() >= 100) {
		if (currentLevel < MAX_LEVELS) {
			gameState = GameState::LEVEL_COMPLETE;
			std::cout << "=== LEVEL " << currentLevel << " COMPLETED! ===" << std::endl;
			std::cout << "Score: " << player.getScore() << " points (100 required)" << std::endl;
		} else {
			gameState = GameState::VICTORY;
			gameRunning = false;
			std::cout << "=== VICTORY! ALL LEVELS COMPLETED! ===" << std::endl;
			std::cout << "Final score: " << player.getScore() << " points!" << std::endl;
		}
	}
}

void GameController::printLevelInfo() const {
	std::cout << "=== LEVEL INFORMATION ===\n";
	std::cout << "Level: " << currentLevel << "/" << MAX_LEVELS << "\n";
	std::cout << "Field size: " << field.getWidth() << "x" << field.getHeight() << "\n";
	std::cout << "Enemies remaining: " << enemyManager->getEnemies().size() << "\n"; 
	std::cout << "Buildings remaining: " << buildingManager->getBuildings().size() << "\n"; 
	std::cout << "Towers: " << towers.size() << "\n";
	std::cout << "Spell cards: " << player.getSpellHand().getSpellCount() << "\n";
}

LevelConfig GameController::getLevelConfig(int level) const {
	LevelConfig config;
	config.level = level;
	
	int baseSize = 12 + (level * 2);
	int baseEnemies = 3 + (level * 2);
	int baseBuildings = 2 + level;
	
	config.fieldWidth = std::min(baseSize, 25);
	config.fieldHeight = std::min(baseSize, 25);
	config.enemyHealth = level * 10;
	config.enemyDamage = level * 3;
	config.playerHealth = 100;
	config.enemyCount = std::min(baseEnemies, 20);
	config.buildingCount = std::min(baseBuildings, 8);
	
	return config;
}

void GameController::removeHalfOfSpells() {
	SpellHand& hand = player.getSpellHand();
	int spellCount = hand.getSpellCount();
	if (spellCount <= 1) return;
	
	int spellsToRemove = spellCount / 2;
	std::cout << "Removing " << spellsToRemove << " out of " << spellCount << " spell cards...\n";
	for (int i = 0; i < spellsToRemove; i++) {
		if (hand.getSpellCount() > 0) hand.removeRandomSpell();
	}
}

bool GameController::isGameRunning() const {
	return gameRunning && player.isAlive() && gameState != GameState::VICTORY;
}

int GameController::getCurrentLevel() const {
	return currentLevel;
}

void GameController::setCurrentLevel(int level) {
	if (level < 1) level = 1;
	if (level > MAX_LEVELS) level = MAX_LEVELS;
	currentLevel = level;
}

GameState GameController::getGameState() const {
	return gameState;
}

bool GameController::isLevelComplete() const {
	return gameState == GameState::LEVEL_COMPLETE;
}

void GameController::startNextLevel() {
	if (currentLevel < MAX_LEVELS && gameState == GameState::LEVEL_COMPLETE) {
		player.setScore(0);
		currentLevel++;
		initializeLevel(currentLevel);
	} else if (currentLevel >= MAX_LEVELS) {
		gameState = GameState::VICTORY;
		gameRunning = false;
	}
}

void GameController::restartGame() {
	currentLevel = 1;
	gameState = GameState::PLAYING;
	gameRunning = true;
	initializeLevel(currentLevel);
}

void GameController::buySpell() {
	if (!isGameRunning() || gameState != GameState::PLAYING) return;
	SpellHand& hand = player.getSpellHand();
	int currentScore = player.getScore();
	if (hand.canBuySpell(currentScore)) {
		int oldCount = hand.getSpellCount();
		hand.buySpell(currentScore);
		player.setScore(currentScore);
		
		std::cout << "New spell acquired! Remaining points: " << player.getScore() << "\n";
	} else {
		std::cout << "Not enough points to buy a spell!\n";
	}
}

const Player& GameController::getPlayer() const {
	return player;
}

const GameField& GameController::getField() const {
	return field;
}

const std::vector<Tower>& GameController::getTowers() const {
	return towers;
}