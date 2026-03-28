#pragma once
#include "SaveManager.hpp"
#include "Field.hpp"
#include "WorldManager.hpp"
#include "Player.hpp"
#include "GameStateManager.hpp"
#include <string>
#include <memory>

class GameSerializer {
public:
	static void serializeGame(SaveManager& save, 
							  const GameStateManager& stateManager,
							  const Player& player,
							  const Field& field,
							  const WorldManager& world);
	
	static void deserializeGame(SaveManager& load,
								GameStateManager& stateManager,
								Player& player,
								std::unique_ptr<Field>& field,
								std::unique_ptr<WorldManager>& world);
};









