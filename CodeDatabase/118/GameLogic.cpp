#include "GameLogic.h"

GameLogic::GameLogic()
	: gameField(nullptr), player(nullptr), spawner(nullptr), tower(nullptr), tableOfSpells(3, std::pair<std::shared_ptr<ISpell>, int>()) {}

GameLogic::GameLogic(int w, int h, std::shared_ptr<Player> player)
	: gameField(std::make_shared<GameField>(w, h)), player(std::make_shared<Player>(*player))
{
	SetTableOfSpells();

	std::pair<int, int> pos(-1, -1);

	std::pair<int, int> spawnerPos = FindPosForSpawning(pos);

	SetEnemySpawner(spawnerPos);
	SpawnEnemy();

	std::pair towerPos = FindPosForSpawning(pos);
	SetTower(towerPos);
}

std::pair<int, int> GameLogic::FindPosForSpawning(std::pair<int, int> pos, std::vector<std::pair<int, int>> indexesRange)
{
	std::random_device rd;
	std::uniform_int_distribution x_dist(std::max(0, indexesRange[0].first), std::min(gameField->GetWidth() - 1, indexesRange[0].second));
	std::uniform_int_distribution y_dist(std::max(0, indexesRange[1].first), std::min(gameField->GetHeight() - 1, indexesRange[1].second));

	while (gameField->GetCellByPosForReading(pos).GetTypeOfCell() != ORDINAL \
		|| gameField->GetCellByPosForReading(pos).GetCellContentForReading() != nullptr)
		pos = std::pair<int, int>(x_dist(rd), y_dist(rd));
	
	return pos;
}

void GameLogic::CreateRandomStartSpell(std::shared_ptr<ISpell>& spell)
{
	std::random_device rd;
	std::uniform_int_distribution range(0, 2);
	
	int key = range(rd);
	spell = tableOfSpells[key].first;
}

bool GameLogic::isEnemyPos(std::pair<int, int> pos)
{
	return gameField->GetCellByPosForReading(pos).GetCellContentForReading() != nullptr && player->GetPosition() != pos \
		&& (!spawner->isDied && spawner->GetPosition() != pos || spawner->isDied) && (!tower->isDied && tower->GetPosition() != pos || tower->isDied);
}

void GameLogic::SpawnEnemy(int cntStep)
{
	if (spawner && !spawner->isDied && cntStep % spawner->GetCoolDown() == 0)
	{
		std::pair<int, int> spawnerPos = spawner->GetPosition();
		int radius = spawner->GetSpawnRadius();

		auto indexesRange = std::vector <std::pair<int, int>>{
			{spawnerPos.first - radius, spawnerPos.first + radius},
			{spawnerPos.second - radius, spawnerPos.second + radius}
		};
		
		std::pair<int, int> enemyPos = FindPosForSpawning(spawnerPos, indexesRange);
		std::shared_ptr<Enemy> spawnedEnemy = std::make_shared<Enemy>(enemyPos);
		gameField->EditCellByPos(spawnedEnemy, enemyPos);
	}
}

void GameLogic::PlayerMove(std::pair<int, int> steps)
{
	std::pair<int, int> playerPos = player->GetPosition();

	if (playerInSlowingCell)
	{
		playerInSlowingCell = false;
		return;
	}

	std::pair<int, int> destMovingPos = std::make_pair<int, int>(playerPos.first + steps.first, playerPos.second + steps.second);

	if (playerPos == destMovingPos || !gameField->IsCorrectPos(destMovingPos))
		throw MovementException("Player can't move. Choose other command.\n");
	
	Cell& destMoving = gameField->GetCellByPos(destMovingPos);
	
	if (destMoving.GetCellContent() != nullptr)
		throw MovementException("Player can't move to occupied cell. Choose other command.\n");

	else if (destMoving.GetTypeOfCell() == IMPASSABLE)
		throw MovementException("Player can't move to wall. Choose other command.\n");

	else
	{
		if (destMoving.GetTypeOfCell() == SLOWING)
			playerInSlowingCell = true;
		
		gameField->SwapCellsContent(playerPos, destMovingPos);

		player->Move(steps);

	}
}

void GameLogic::PlayerAttack(std::shared_ptr<Entity> entity, int pointsForKilling)
{
	if (entity->isDied)
		throw AttackException("This entity has already died. Choose other command.\n");

	int entityHealth = entity->GetHealth();
	
	player->Attack(entity);

	if (entity->GetHealth() == entityHealth)
		throw AttackException("Attacked entity is too far away.\n");

	else if (entity->isDied)
	{
		gameField->EditCellByPos(entity, entity->GetPosition(), true);
		player->ChangeScore(pointsForKilling);
	}
}

void GameLogic::PlayerBuySpell(std::shared_ptr<ISpell> spell, int cost)
{
	if (!player->BuySpell(spell, cost))
		throw SpellException("Purchase failed. Check your balance and hand capacity, maybe these factors hinder the purchase.\n");
}

void GameLogic::PlayerCastSpell(int number, std::pair<int, int> targetPos)
{
	if (!player->CastSpellWithNumber(number, targetPos))
		throw SpellException("It is impossible to cast of this spell. Try again.\n");
}

std::vector<std::pair<int, int>> GameLogic::GetEnemyPositions()
{
	std::vector<std::pair<int, int>> positions{};

	for (int y = 0; y < gameField->GetHeight(); ++y)
	{
		for (int x = 0; x < gameField->GetWidth(); ++x)
		{
			std::pair<int, int> currPos = std::pair<int, int>(x, y);
			Cell& cell = gameField->GetCellByPos(currPos);
			if (isEnemyPos(currPos))
				positions.push_back(currPos);
		}
	}

	return positions;
}

void GameLogic::EnemiesMove()
{
	std::vector<std::pair<int, int>> enemyPositions = GetEnemyPositions();
	
	for (int i = 0; i < enemyPositions.size(); ++i)
	{
		std::pair<int, int> enemyPos = enemyPositions[i];
		std::shared_ptr<Entity> enemy = gameField->GetCellByPos(enemyPos).GetCellContent();
		
		std::pair<int, int> desiredPos = player->GetPosition();
		int dx = std::abs(enemyPos.first - desiredPos.first);
		int dy = std::abs(enemyPos.second - desiredPos.second);


		if (dx <= 1 && dy <= 1)
		{
			enemy->Attack(player);
			continue;
		}

		int xStep = enemyPos.first < desiredPos.first ? 1 : enemyPos.first == desiredPos.first ? 0 : -1;
		int yStep = enemyPos.second < desiredPos.second ? 1 : enemyPos.second == desiredPos.second ? 0 : -1;
		
		Cell& destCellH = gameField->GetCellByPos(std::pair<int, int>(enemyPos.first + xStep, enemyPos.second));
		Cell& destCellV = gameField->GetCellByPos(std::pair<int, int>(enemyPos.first, enemyPos.second + yStep));
		Cell& destCellD = gameField->GetCellByPos(std::pair<int, int>(enemyPos.first + xStep, enemyPos.second + yStep));

		bool canMoveH = destCellH.GetTypeOfCell() != IMPASSABLE && destCellH.GetCellContentForReading() == nullptr;

		bool canMoveV = destCellV.GetTypeOfCell() != IMPASSABLE && destCellV.GetCellContentForReading() == nullptr;

		bool canMoveD = destCellD.GetTypeOfCell() != IMPASSABLE && destCellD.GetCellContentForReading() == nullptr;

		std::pair<int, int> steps(0, 0);
		std::vector<std::pair<int, int>> workarounds;

		if (dx > 1 && dy == 0)
		{
			if (!canMoveH)
				workarounds = std::vector<std::pair<int, int>>{ { xStep, yStep }, { xStep, -yStep }, { 0, yStep }, { 0, -yStep } };

			else
				steps = std::make_pair(xStep, yStep);
		}
			
		else if (dx == 0 && dy > 1)
		{
			if (!canMoveV)
				workarounds = std::vector<std::pair<int, int>>{ { xStep, yStep }, { -xStep, yStep }, { xStep, 0 }, { -xStep, 0 } };

			else
				steps = std::make_pair(xStep, yStep);
		}

		else if (dx >= 1 && dy >= 1)
		{
			if (!canMoveD)
				workarounds = std::vector<std::pair<int, int>>{ { xStep, 0 }, { 0, yStep } };

			else
				steps = std::make_pair(xStep, yStep);
		}

		else
			continue;

		if (steps.first == 0 && steps.second == 0)
		{
			for (auto& move : workarounds)
			{
				auto destPos = std::pair<int, int>(enemyPos.first + move.first, enemyPos.second + move.second);

				if (destPos.first < 0 || destPos.first > gameField->GetWidth() - 1 || destPos.second < 0 || destPos.second > gameField->GetHeight() - 1)
					continue;

				Cell& destCell = gameField->GetCellByPos(destPos);

				if (destCell.GetTypeOfCell() == IMPASSABLE || destCell.GetCellContentForReading() != nullptr)
					continue;

				steps = std::make_pair(move.first, move.second);
				break;
			}
		}

		if (steps.first != 0 || steps.second != 0)
		{
			enemy->Move(steps);
			
			if (gameField->GetCellByPos(enemy->GetPosition()).GetTypeOfCell() == TRAP)
			{
				enemy->TakeDamage(TRAP_DAMAGE + gameField->GetCellByPos(enemy->GetPosition()).GetBuff());
				gameField->GetCellByPos(enemy->GetPosition()).SetTypeOfCell(ORDINAL);

				if (enemy->isDied)
				{
					gameField->EditCellByPos(enemy, enemy->GetPosition(), true);
					return;
				}
			}

			gameField->SwapCellsContent(enemyPos, enemy->GetPosition());
			enemyPositions[i] = enemy->GetPosition();
		}
	}
}

void GameLogic::SetTableOfSpells(int radius, int buff)
{
	tableOfSpells = std::vector<std::pair<std::shared_ptr<ISpell>, int>>{
		{std::make_shared<SpellOfDirectDamage>(gameField, radius, buff), DIRECT_COST},
		{std::make_shared<SpellOfAreaDamage>(gameField, radius, buff), AREA_COST},
		{std::make_shared<SpellOfSpawningTrap>(gameField, radius, buff), TRAP_COST}
	};
}

void GameLogic::SetGameField(int w, int h, const std::map<TypeOfCell, std::vector<std::pair<int, int>>>& unusualCellPositions, std::vector<std::shared_ptr<Entity>> enemies)
{
	gameField = std::make_shared<GameField>(w, h);

	for (const auto& [key, positions] : unusualCellPositions)
	{
		for (const auto& pos : positions)
			gameField->GetCellByPos(pos).SetTypeOfCell(key);
	}

	for (const auto& enemy : enemies)
		gameField->EditCellByPos(enemy, enemy->GetPosition(), enemy->isDied);
}

void GameLogic::SetPlayer(int health, std::pair<int, int> pos, int damage, int score, TypeOfAttack type, Hand hand)
{
	player = std::make_shared<Player>(health, pos, damage, score, type, hand);
	gameField->EditCellByPos(player, pos);
}

void GameLogic::SetEnemySpawner(std::pair<int, int> pos, int health, int coolDown, int radius)
{
	spawner = std::make_shared<EnemySpawner>(health, pos, coolDown, radius);
	spawner->isDied = spawner->GetHealth() == 0;
	gameField->EditCellByPos(spawner, pos, spawner->isDied);
}

void GameLogic::SetTower(std::pair<int, int> pos, int health, int radius, int debuff, bool canAttack)
{
	tower = std::make_shared<Tower>(health, pos, radius, debuff, canAttack);
	tower->isDied = tower->GetHealth() == 0;
	gameField->EditCellByPos(tower, pos, tower->isDied);
}