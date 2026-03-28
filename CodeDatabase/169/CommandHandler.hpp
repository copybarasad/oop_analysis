#pragma once
#include "WorldManager.hpp"
#include "Player.hpp"
#include "Coords.hpp"

class CommandHandler {
public:
	static bool handleCommand(char input, WorldManager& world, Player& player);
	static bool handlePlayerMove(char direction, WorldManager& world, Player& player);
	static bool handlePlayerAttack(WorldManager& world, Player& player);
	static bool handleSpellCast(WorldManager& world, const Coords& playerPos);
	static bool handleModeToggle(Player& player);
};









