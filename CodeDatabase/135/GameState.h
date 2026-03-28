#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "GameField.h"
#include "EntityManager.h"

class GameState {
private:
	GameField field;
	EntityManager entity_manager;
	int player_index;
	unsigned int game_turn;

public:
	GameState(int field_width, int field_height, RandomGenerator& rng);
	
	GameState(GameState&&) noexcept = default;
	GameState& operator=(GameState&&) noexcept = default;
	
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
	
	GameField& getField();
	const GameField& getField() const;
	
	EntityManager& getEntityManager();
	const EntityManager& getEntityManager() const;
	
	int getPlayerIndex() const;
	void setPlayerIndex(int index);
	
	unsigned int getGameTurn() const;
	void setGameTurn(unsigned int turn);
	void incrementTurn();
	
	int& getPlayerIndexRef();
	unsigned int& getGameTurnRef();
};

#endif
