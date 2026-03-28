#ifndef GAME_logic_H
#define GAME_logic_H

#include <random>
#include <memory>
#include <map>

#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "Tower.h"

#include "GameField.h"

#include "Spells.h"

#include "GameExceptions.h"

class GameLogic
{
	std::shared_ptr<GameField> gameField;
	std::shared_ptr<Player> player;
	std::shared_ptr<EnemySpawner> spawner;
	std::shared_ptr<Tower> tower;
	bool playerInSlowingCell = false;
	std::vector<std::pair<std::shared_ptr<ISpell>, int>> tableOfSpells;
	std::pair<int, int> FindPosForSpawning(std::pair<int, int>, std::vector<std::pair<int, int>> = std::vector<std::pair<int, int>>{ {0, 30}, {0, 30} });
	void CreateRandomStartSpell(std::shared_ptr<ISpell>&);
	bool isEnemyPos(std::pair<int, int>);
public:
	GameLogic();
	GameLogic(int, int, std::shared_ptr<Player>);

	void SpawnEnemy(int cnt = 0);

	void PlayerMove(std::pair<int, int>);
	void PlayerAttack(std::shared_ptr<Entity>, int);
	void PlayerBuySpell(std::shared_ptr<ISpell>, int);
	void PlayerCastSpell(int, std::pair<int, int>);

	std::vector<std::pair<int, int>> GetEnemyPositions();
	void EnemiesMove();

	std::vector<std::pair<std::shared_ptr<ISpell>, int>>& GetTableOfSpells() { return tableOfSpells; };
	void SetTableOfSpells(int=STANDART_SPELL_RADIUS, int=0);

	std::shared_ptr<GameField> GetGameField() { return gameField;  };
	void SetGameField(int, int, const std::map<TypeOfCell, std::vector<std::pair<int, int>>>&, std::vector<std::shared_ptr<Entity>> = {});

	std::shared_ptr<Player> GetPlayer() { return player; };
	void SetPlayer(int, std::pair<int, int>, int, int, TypeOfAttack, Hand);

	std::shared_ptr<EnemySpawner> GetEnemySpawner() { return spawner; };
	void SetEnemySpawner(std::pair<int, int>, int=SPAWNER_HEALTH, int=SPAWNER_COOLDOWN, int=SPAWNER_RADIUS);

	std::shared_ptr<Tower> GetTower() { return tower; };
	void SetTower(std::pair<int, int>, int=TOWER_HEALTH, int=STANDART_SPELL_RADIUS, int=TOWER_DEBUFF, bool=true);
};

#endif

