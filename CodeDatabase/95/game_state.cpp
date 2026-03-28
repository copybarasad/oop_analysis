#include "game_state.hpp"
#include "game_objects/player.hpp"
#include "game_objects/enemy.hpp"
#include "game_objects/tower.hpp"
#include "game_objects/spells/aoe_spell.hpp"
#include "game_objects/spells/damage_spell.hpp"
#include "game_objects/spells/trap_spell.hpp"

GameState::GameState() : status(GameStatus::Running), turn_counter(0) {}

eManager& GameState::getEntityManager() { 
    return entityManager; 
}

const eManager& GameState::getEntityManager() const { 
    return entityManager; 
}

GameStatus GameState::getStatus() const { 
    return status;
}

int GameState::getTurnCounter() const {
    return turn_counter;
}

bool GameState::isRunning() const {
    return status == GameStatus::Running;
}

void GameState::setStatus(GameStatus s) {
    status = s;
}

void GameState::setTurnCounter(int turns) {
    turn_counter = turns;
}

void GameState::incrementTurnCounter() {
    turn_counter++;
}
