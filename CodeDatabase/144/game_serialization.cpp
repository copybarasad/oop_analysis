#include "game.h"
#include "spell_state.h"
#include "ally.h"
#include "enemy.h"
#include "enemybuilding.h"
#include "enemytower.h"

std::vector<SpellState> Game::captureAvailableSpellStates() const {
	std::vector<SpellState> states;
	states.reserve(availableSpells.size());
	SpellSerializer serializer;
	for (const auto& spell : availableSpells) {
		states.push_back(serializer.serialize(*spell));
	}
	return states;
}

void Game::restoreAvailableSpells(const std::vector<SpellState>& states) {
	availableSpells.clear();
	SpellSerializer serializer;
	for (const auto& state : states) {
		auto spell = serializer.deserialize(state);
		availableSpells.push_back(std::move(spell));
	}
}

GameState Game::buildGameState() const {
	GameState snapshot;
	snapshot.levelIndex = currentLevelIndex;
	snapshot.turnCounter = turnCounter;
	snapshot.playerActionTaken = playerActionTaken;
	snapshot.enemiesKilledSinceLastSpell = enemiesKilledSinceLastSpell;
	snapshot.baseFieldWidth = baseFieldWidth;
	snapshot.baseFieldHeight = baseFieldHeight;
	snapshot.currentEnemyHealth = currentEnemyHealth;
	snapshot.currentEnemyDamage = currentEnemyDamage;
	snapshot.fieldSnapshot = field.createSnapshot();
	snapshot.playerSnapshot = player.createSnapshot();
	snapshot.availableSpells = captureAvailableSpellStates();

	for (const auto& enemy : enemyManager.getEnemies()) {
		snapshot.enemySnapshots.push_back(enemy.createSnapshot());
	}

	for (const auto& building : buildingManager.getBuildings()) {
		snapshot.buildingSnapshots.push_back(building.createSnapshot());
	}

	for (const auto& tower : buildingManager.getTowers()) {
		snapshot.towerSnapshots.push_back(tower.createSnapshot());
	}

	for (const auto& trap : traps) {
		snapshot.trapSnapshots.push_back(trap.createSnapshot());
	}

	for (const auto& ally : allies) {
		snapshot.allySnapshots.push_back(ally.createSnapshot());
	}

	snapshot.permanentSpellEnhancements = permanentSpellEnhancements;

	return snapshot;
}

void Game::restoreFromState(const GameState& state) {
	currentLevelIndex = state.levelIndex;
	turnCounter = state.turnCounter;
	playerActionTaken = state.playerActionTaken;
	enemiesKilledSinceLastSpell = state.enemiesKilledSinceLastSpell;
	baseFieldWidth = state.baseFieldWidth;
	baseFieldHeight = state.baseFieldHeight;
	currentEnemyHealth = state.currentEnemyHealth;
	currentEnemyDamage = state.currentEnemyDamage;
	field.applySnapshot(state.fieldSnapshot);

	player.applySnapshot(state.playerSnapshot);
	restoreAvailableSpells(state.availableSpells);

	auto& enemies = enemyManager.getEnemies();
	enemies.clear();
	for (const auto& enemySnapshot : state.enemySnapshots) {
		enemies.push_back(Enemy::fromSnapshot(enemySnapshot));
	}

	auto& buildings = buildingManager.getBuildings();
	buildings.clear();
	for (const auto& buildingSnapshot : state.buildingSnapshots) {
		buildings.push_back(EnemyBuilding::fromSnapshot(buildingSnapshot));
	}

	auto& towers = buildingManager.getTowers();
	towers.clear();
	for (const auto& towerSnapshot : state.towerSnapshots) {
		towers.push_back(EnemyTower::fromSnapshot(towerSnapshot));
	}

	traps.clear();
	for (const auto& trapSnapshot : state.trapSnapshots) {
		traps.push_back(Trap::fromSnapshot(trapSnapshot));
	}

	allies.clear();
	for (const auto& allySnapshot : state.allySnapshots) {
		allies.push_back(Ally::fromSnapshot(allySnapshot));
	}

	permanentSpellEnhancements = state.permanentSpellEnhancements;

	levelInProgress = true;
	gameRunning = true;
	campaignActive = true;
	awaitingUpgradeSelection = false;
	awaitingRestartDecision = false;
}


