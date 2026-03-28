#include "CommandHandler.hpp"
#include "SpellManager.hpp"
#include "ISpell.hpp"
#include "DirectDamageSpell.hpp"
#include "AreaDamageSpell.hpp"
#include "TrapSpell.hpp"
#include <iostream>
#include <limits>

bool CommandHandler::handleCommand(char input, WorldManager& world, Player& player) {
	switch (input) {
		case 'w':
			return handlePlayerMove('w', world, player);
		case 's':
			return handlePlayerMove('s', world, player);
		case 'a':
			return handlePlayerMove('a', world, player);
		case 'd':
			return handlePlayerMove('d', world, player);
		case 'f':
			return handlePlayerAttack(world, player);
		case 't':
			handleModeToggle(player);
			return false;
		case 'c':
			return handleSpellCast(world, world.getPlayerPos());
		case 'q':
			return false;
		default:
			std::cout << "|| неизвестная команда.";
			std::cin.ignore();
			std::cin.get();
			return false;
	}
}

bool CommandHandler::handlePlayerMove(char direction, WorldManager& world, Player& player) {
	int dx = 0, dy = 0;
	switch (direction) {
		case 'w': dy = -1; break;
		case 's': dy = 1; break;
		case 'a': dx = -1; break;
		case 'd': dx = 1; break;
	}
	
	MoveResult result = world.movePlayer(dx, dy, player);
	switch (result) {
		case MoveResult::Moved:
			return true;
		case MoveResult::Slowed:
			return true;
		case MoveResult::OccupiedEnemy:
			return true;
		case MoveResult::Blocked:
			std::cout << "|| путь заблокирован.";
			std::cin.ignore();
			std::cin.get();
			return false;
		case MoveResult::OutOfBounds:
			std::cout << "|| выход за границы поля.";
			std::cin.ignore();
			std::cin.get();
			return false;
	}
	return false;
}

bool CommandHandler::handlePlayerAttack(WorldManager& world, Player& player) {
	if (world.playerAttack(player)) {
		return true;
	} else {
		std::cout << "|| нет врагов в зоне атаки.";
		std::cin.ignore();
		std::cin.get();
		return false;
	}
}

bool CommandHandler::handleSpellCast(WorldManager& world, const Coords& playerPos) {
	size_t spellIndex;
	int targetX, targetY;
	std::cin >> spellIndex >> targetX >> targetY;
	
	SpellManager& spells = world.getSpellManager();
	if (spellIndex >= spells.getSpellCount()) {
		std::cout << "|| Неверный номер заклинания";
		std::cin.ignore();
		std::cin.get();
		return false;
	}
	
	Coords targetPos{targetX, targetY};
	const ISpell* spell = spells.getSpell(spellIndex);
	
	if (spell) {
		int range = spell->getRange();
		if (!spells.isInRange(playerPos, targetPos, range)) {
			std::cout << "|| Цель слишком далеко! Радиус заклинания: " << range ;
			std::cin.ignore();
			std::cin.get();
			return false;
		}
	}
	
	if (spells.useSpell(spellIndex, playerPos, targetPos)) {
		return true;
	} else {
		std::cout << "|| Заклинание не было использовано (нет цели).";
		std::cin.ignore();
		std::cin.get();
		return false;
	}
}

bool CommandHandler::handleModeToggle(Player& player) {
	player.toggleMode();
	std::cout << "|| Режим атаки изменён на " 
			  << (player.getMode() == AttackMode::Melee ? "ближний бой" : "дальний бой");
	std::cin.ignore();
	std::cin.get();
	return false;
}









