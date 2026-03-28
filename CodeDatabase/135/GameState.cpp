#include "GameState.h"

GameState::GameState(int field_width, int field_height, RandomGenerator& rng)
	: field(field_width, field_height, rng),
	  entity_manager(),
	  player_index(-1),
	  game_turn(0) {
}

GameField& GameState::getField() {
	return field;
}

const GameField& GameState::getField() const {
	return field;
}

EntityManager& GameState::getEntityManager() {
	return entity_manager;
}

const EntityManager& GameState::getEntityManager() const {
	return entity_manager;
}

int GameState::getPlayerIndex() const {
	return player_index;
}

void GameState::setPlayerIndex(int index) {
	player_index = index;
}

unsigned int GameState::getGameTurn() const {
	return game_turn;
}

void GameState::setGameTurn(unsigned int turn) {
	game_turn = turn;
}

void GameState::incrementTurn() {
	game_turn++;
}

int& GameState::getPlayerIndexRef() {
	return player_index;
}

unsigned int& GameState::getGameTurnRef() {
	return game_turn;
}
