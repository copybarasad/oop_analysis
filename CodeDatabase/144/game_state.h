#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <map>
#include <vector>
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "enemybuilding.h"
#include "enemytower.h"
#include "trap.h"
#include "ally.h"
#include "spell_state.h"

class GameState {
public:
	int levelIndex = 1;
	int turnCounter = 0;
	bool playerActionTaken = false;
	int enemiesKilledSinceLastSpell = 0;
	int baseFieldWidth = 0;
	int baseFieldHeight = 0;
	int currentEnemyHealth = 30;
	int currentEnemyDamage = 10;
	Field::Snapshot fieldSnapshot;
	Player::Snapshot playerSnapshot;
	std::vector<Enemy::Snapshot> enemySnapshots;
	std::vector<EnemyBuilding::Snapshot> buildingSnapshots;
	std::vector<EnemyTower::Snapshot> towerSnapshots;
	std::vector<Trap::Snapshot> trapSnapshots;
	std::vector<Ally::Snapshot> allySnapshots;
	std::vector<SpellState> availableSpells;
	std::map<SpellType, int> permanentSpellEnhancements;
};

#endif

