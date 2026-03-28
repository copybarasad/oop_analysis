#include "game.h"
#include "spell_state.h"
#include "file_modification_exception.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>

Game::LevelDefinition Game::buildLevelDefinition(int levelIndex) const {
	LevelDefinition definition;
	definition.fieldWidth = std::clamp(baseFieldWidth + (levelIndex - 1) * 2, 10, 30);
	definition.fieldHeight = std::clamp(baseFieldHeight + (levelIndex - 1) * 2, 10, 30);
	definition.initialEnemies = 3 + (levelIndex - 1) * 2;
	definition.buildingCount = 2 + (levelIndex - 1);
	definition.towerCount = 1 + levelIndex / 2;
	definition.enemyHealth = 30 + (levelIndex - 1) * 10;
	definition.enemyDamage = 10 + (levelIndex - 1) * 2;
	definition.buildingHealth = 100 + (levelIndex - 1) * 20;
	definition.towerHealth = 150 + (levelIndex - 1) * 30;
	definition.towerRange = 4 + (levelIndex / 3);
	definition.towerDamage = 8 + (levelIndex - 1) * 2;
	return definition;
}

void Game::resetWorldState() {
	enemyManager.getEnemies().clear();
	buildingManager.getBuildings().clear();
	buildingManager.getTowers().clear();
	traps.clear();
	allies.clear();
}

void Game::startLevel(int levelIndex, bool fromLoad) {
	currentLevelIndex = levelIndex;
	gameRunning = true;
	levelInProgress = true;
	awaitingUpgradeSelection = false;
	playerActionTaken = false;
	enemiesKilledSinceLastSpell = 0;
	if (fromLoad) {
		return;
	}

	resetWorldState();
	LevelDefinition definition = buildLevelDefinition(levelIndex);
	currentEnemyHealth = definition.enemyHealth;
	currentEnemyDamage = definition.enemyDamage;
	field = Field(definition.fieldWidth, definition.fieldHeight);
	spawnPlayer();
	enemyManager.spawnInitialEnemies(field, buildingManager, player, definition.initialEnemies,
	                                 definition.enemyHealth, definition.enemyDamage, &eventBus);
	int totalStructures = definition.buildingCount + definition.towerCount;
	buildingManager.spawnBuildings(field, player, totalStructures,
	                               definition.buildingHealth, definition.towerHealth,
	                               definition.towerRange, definition.towerDamage);
	std::cout << "Level " << levelIndex << " started. Field size "
	          << definition.fieldWidth << "x" << definition.fieldHeight << ".\n";
}

bool Game::isLevelCleared() const {
	const auto& enemies = enemyManager.getEnemies();
	bool anyEnemyAlive = std::any_of(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
		return enemy.isAlive();
	});
	return !anyEnemyAlive &&
	       buildingManager.getBuildings().empty() &&
	       buildingManager.getTowers().empty();
}

void Game::removeHalfOfPlayerSpells() {
	size_t toRemove = player.getHand().getSpellCount() / 2;
	player.getHand().removeRandomSpells(toRemove);
}

void Game::handleLevelCompletion() {
	if (!levelInProgress) {
		return;
	}
	levelInProgress = false;
	awaitingUpgradeSelection = true;
	player.setCurrentHealth(player.getMaxHealth());
	player.setManaValues(player.getMaxMana(), player.getMaxMana());
	removeHalfOfPlayerSpells();
	std::cout << "Level " << currentLevelIndex << " cleared! Choose an upgrade before proceeding.\n";
	promptUpgradeSelection();
}

void Game::promptUpgradeSelection() {
	static constexpr int OPTION_COUNT = 4;
	int choice = 0;
	while (true) {
		std::cout << "Upgrade options:\n";
		std::cout << " 1) +20 maximum health\n";
		std::cout << " 2) +15 maximum mana\n";
		std::cout << " 3) +5 melee & ranged damage\n";
		std::cout << " 4) Empower a random spell (enhancement +1)\n";
		std::cout << "Select upgrade (1-" << OPTION_COUNT << "): ";
		if (std::cin >> choice && choice >= 1 && choice <= OPTION_COUNT) {
			break;
		}
		std::cout << "Invalid choice. Try again.\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	applyUpgradeChoice(choice);
	awaitingUpgradeSelection = false;
	startLevel(currentLevelIndex + 1);
}

void Game::applyUpgradeChoice(int choice) {
	switch (choice) {
		case 1: {
			player.setMaximumHealth(player.getMaxHealth() + 20);
			player.setCurrentHealth(player.getMaxHealth());
			std::cout << "Maximum health increased to " << player.getMaxHealth() << ".\n";
			break;
		}
		case 2: {
			player.setManaValues(player.getMaxMana() + 15, player.getMaxMana() + 15);
			std::cout << "Maximum mana increased to " << player.getMaxMana() << ".\n";
			break;
		}
		case 3: {
			player.setMeleeAttackDamage(player.getMeleeAttackDamageValue() + 5);
			player.setRangedAttackDamage(player.getRangedAttackDamageValue() + 5);
			std::cout << "Damage increased.\n";
			break;
		}
		case 4: {
			std::vector<SpellType> availableTypes = {
				SpellType::DIRECT_DAMAGE,
				SpellType::AREA_DAMAGE,
				SpellType::TRAP,
				SpellType::SUMMON
			};
			
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(0, availableTypes.size() - 1);
			SpellType selectedType = availableTypes[dist(gen)];
			
			permanentSpellEnhancements[selectedType] += 1;
			
			auto& spells = player.getHand().getSpells();
			for (auto& spell : spells) {
				if (spell->getSpellType() == selectedType) {
					spell->applyPermanentEnhancement(1);
				}
			}
			
			std::string spellName;
			switch (selectedType) {
				case SpellType::DIRECT_DAMAGE: spellName = "Direct Damage"; break;
				case SpellType::AREA_DAMAGE: spellName = "Area Damage"; break;
				case SpellType::TRAP: spellName = "Trap"; break;
				case SpellType::SUMMON: spellName = "Summon Ally"; break;
				default: spellName = "Unknown"; break;
			}
			
			std::cout << "Spell type \"" << spellName << "\" permanently enhanced by 1 level "
			          << "(total: " << permanentSpellEnhancements[selectedType] << ").\n";
			break;
		}
		default:
			break;
	}
}

void Game::onPlayerDefeat() {
	awaitingRestartDecision = true;
	char choice = 'n';
	while (true) {
		std::cout << "You have been defeated. Restart level? (y/n): ";
		if (std::cin >> choice) {
			if (choice == 'y' || choice == 'Y') {
				player.setCurrentHealth(player.getMaxHealth());
				player.setManaValues(player.getMaxMana(), player.getMaxMana());
				awaitingRestartDecision = false;
				startLevel(currentLevelIndex);
				return;
			}
			if (choice == 'n' || choice == 'N') {
				campaignActive = false;
				gameRunning = false;
				return;
			}
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void Game::saveGameState() {
	try {
		persistence.save(buildGameState(), saveFilePath);
		logEvent(GameEventType::System, "Game saved to " + saveFilePath);
	} catch (const SaveGameException& ex) {
		logEvent(GameEventType::System, std::string("Failed to save game: ") + ex.what());
	} catch (const FileAccessException& ex) {
		logEvent(GameEventType::System, std::string("File access error while saving: ") + ex.what());
	} catch (const std::exception& ex) {
		logEvent(GameEventType::System, std::string("Unexpected error while saving: ") + ex.what());
	} catch (...) {
		logEvent(GameEventType::System, "Unknown error occurred while saving.");
	}
}

void Game::loadGameState() {
	try {
		GameState state = persistence.load(saveFilePath);
		restoreFromState(state);
		logEvent(GameEventType::System, "Game loaded from " + saveFilePath);
	} catch (const FileModificationException& ex) {
		std::ostringstream ss;
		ss << "Save file integrity check failed. File: " << ex.getFilePath()
		   << ", expected checksum: " << ex.getExpectedChecksum()
		   << ", actual checksum: " << ex.getActualChecksum();
		logEvent(GameEventType::System, ss.str());
	} catch (const CorruptedDataException& ex) {
		logEvent(GameEventType::System, std::string("Corrupted save file detected: ") + ex.what());
	} catch (const InvalidDataException& ex) {
		logEvent(GameEventType::System, std::string("Invalid data in save file: ") + ex.what());
	} catch (const FileAccessException& ex) {
		logEvent(GameEventType::System, std::string("File access error while loading: ") + ex.what());
	} catch (const LoadGameException& ex) {
		logEvent(GameEventType::System, std::string("Failed to load game: ") + ex.what());
	} catch (const std::exception& ex) {
		logEvent(GameEventType::System, std::string("Unexpected error while loading: ") + ex.what());
	} catch (...) {
		logEvent(GameEventType::System, "Unknown error occurred while loading.");
	}
}

void Game::saveCampaign() {
	saveGameState();
}

void Game::loadCampaign() {
	loadGameState();
}


