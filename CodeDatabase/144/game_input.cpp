#include "game.h"
#include "console_command_reader.h"
#include <iostream>
#include <limits>
#include <sstream>

void Game::processCommand(const GameCommand& command) {
	playerActionTaken = false;

	int x = player.getX();
	int y = player.getY();
	int newX = x + command.deltaX;
	int newY = y + command.deltaY;

	if (player.isSlowed() && command.type == CommandType::Move) {
		std::cout << "You are slowed and cannot move this turn!\n";
		player.setSlowed(false);
		playerActionTaken = true;
		return;
	}

	switch (command.type) {
		case CommandType::Move:
			if (enemyManager.attackEnemyAtPosition(newX, newY, player, &eventBus)) {
				playerActionTaken = true;
				giveSpellForEnemyKill();
			} else if (buildingManager.isCellOccupiedByBuilding(newX, newY)) {
				std::cout << "Cannot move there - building in the way!\n";
			} else if (field.canMoveTo(newX, newY)) {
				player.setPosition(newX, newY);
				processCellEffects(newX, newY);
				playerActionTaken = true;
				std::ostringstream ss;
				ss << "Player moved to (" << newX << ", " << newY << ")";
				logEvent(GameEventType::Movement, ss.str());
			} else {
				std::cout << "Cannot move there!\n";
			}
			break;

		case CommandType::SwitchMode:
			player.switchCombatMode();
			std::cout << "Switched to "
				<< (player.getCombatMode() == CombatMode::MELEE ? "Melee" : "Ranged")
				<< " mode (turn consumed)\n";
			playerActionTaken = true;
			logEvent(GameEventType::System, "Player switched combat mode");
			break;

		case CommandType::RangedAttack:
			if (player.getCombatMode() == CombatMode::RANGED && (command.deltaX != 0 || command.deltaY != 0)) {
				enemyManager.performRangedAttack(field, player, command.deltaX, command.deltaY, &eventBus);
				playerActionTaken = true;
				giveSpellForEnemyKill();
				std::ostringstream ss;
				ss << "Ranged attack from (" << x << ", " << y << ") direction (" << command.deltaX << ", " << command.deltaY << ")";
				logEvent(GameEventType::Damage, ss.str());
			} else {
				std::cout << "You can only use ranged attacks in ranged mode!\n";
			}
			break;

		case CommandType::CastSpell: {
			displaySpells();
			if (player.getHand().getSpellCount() > 0) {
				int selectedIndex = command.spellIndex;
				if (selectedIndex < 0) {
					std::cout << "Select spell to cast (index): ";
					if (!(std::cin >> selectedIndex)) {
						std::cout << "Invalid input! Please enter a number.\n";
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						break;
					}
				}

				if (selectedIndex >= 0 && selectedIndex < player.getHand().getSpellCount()) {
					if (castSpell(selectedIndex)) {
						playerActionTaken = true;
						logEvent(GameEventType::Spell, "Spell cast from hand slot " + std::to_string(selectedIndex));
					}
				} else {
					std::cout << "Invalid spell index!\n";
				}
			}
			break;
		}

		case CommandType::BuySpell:
			if (buySpell()) {
				playerActionTaken = true;
				logEvent(GameEventType::Resource, "Player bought a spell");
			}
			break;

		case CommandType::ShowInfo:
			displaySpells();
			std::cout << "Mana: " << player.getMana() << "/" << player.getMaxMana() << "\n";
			std::cout << "Score: " << player.getScore() << " | Next spell at: "
				<< (ENEMIES_PER_SPELL - enemiesKilledSinceLastSpell) << " kills\n";
			break;

		case CommandType::Save:
			saveGameState();
			logEvent(GameEventType::System, "Manual save requested");
			break;

		case CommandType::Load:
			loadGameState();
			logEvent(GameEventType::System, "Manual load requested");
			break;

		case CommandType::Quit:
			gameRunning = false;
			campaignActive = false;
			std::cout << "Game ended by player.\n";
			logEvent(GameEventType::System, "Game terminated by player");
			break;

		case CommandType::Help:
			displayHelp();
			break;

		case CommandType::None:
		case CommandType::Invalid:
		default:
			std::cout << "Unknown command. Press 'h' for help.\n";
			break;
	}
}

void Game::processInput(char input) {
	ConsoleCommandReader reader(controls);
	processCommand(reader.commandFromChar(input));
}