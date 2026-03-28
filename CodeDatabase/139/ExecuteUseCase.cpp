#include "ExecuteUseCase.h"
#include <cstdlib>
#include <ctime>
#include <iostream>


ExecuteUseCase::ExecuteUseCase(int width, int height, int cellSize)
{
	gameBoardSettings = new GBSettings(width, height, cellSize);
	jsonManager = new JsonManager("GameData/game_save.json");
}


void ExecuteUseCase::saveGame()
{
	try {
		jsonManager->jsonDump(*gameBoard, gameBoardSettings->getLevel());
	}
	catch (std::runtime_error& e)
	{
		std::cerr << u8"Ошибка при сохранении игры. " << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << u8"Ошибка при сохранении игры. " << e.what() << std::endl;
	}
}


bool ExecuteUseCase::loadGame(unsigned short& level)
{
	GameBoard* gameBoardnew = nullptr;

	try
	{
		nlohmann::json js = jsonManager->jsonLoad();

		gameBoardnew = new GameBoard(js);

		if (gameBoard)
		{
			delete gameBoard;
			gameBoard = nullptr;
		}

		gameBoard = gameBoardnew;

		if (js.contains("level"))
		{
			level = js["level"];
			gameBoardSettings->setLevel(level);
		}
		else
			throw std::runtime_error(u8"Отсутствует поле level");
	}
	catch (std::exception& e)
	{
		std::cerr << u8"Ошибка при загрузке игры. " << e.what() << std::endl;
		return false;
	}

	return true;
}


Point ExecuteUseCase::newGame(int level, int addHealth, int addDamage)
{

	GameBoard* newGameBoard;
	gameBoardSettings->setLevel(level);
	if (level > 1) {
		gameBoardSettings->addPlayerHealth(addHealth);
		gameBoardSettings->addPlayerDamage(addDamage);
	}

	newGameBoard = new GameBoard();

	newGameBoard->createField(gameBoardSettings->getFieldWidth(), gameBoardSettings->getFieldHeight(), gameBoardSettings->getFieldCellSize());
	newGameBoard->spawnTower(gameBoardSettings->getTowerHealth(), gameBoardSettings->getTowerRadius(), gameBoardSettings->getTowerSpawnTime());

	if (gameBoard && level != 1)
	{
		gameBoard->getPlayer()->getHand()->removeRandomSpells(gameBoard->getPlayer()->getHand()->getSpellsCount() / 2);
		newGameBoard->createPlayer(*gameBoard->getPlayer());
		newGameBoard->getPlayer()->addScore(-newGameBoard->getPlayer()->getScore());
		newGameBoard->getPlayer()->setHealth(gameBoardSettings->getPlayerHealth());
		newGameBoard->getPlayer()->setDamage(gameBoardSettings->getPlayerDamage());
	}
	else {
		newGameBoard->createPlayer(gameBoardSettings->getPlayerHealth(), gameBoardSettings->getPlayerDamage());
	}

	
	if (gameBoard) {
		delete gameBoard;
		gameBoard = nullptr;
	}

	gameBoard = newGameBoard;
	
	return Point(gameBoardSettings->getFieldWidth() * gameBoardSettings->getFieldCellSize(), gameBoardSettings->getFieldHeight() * gameBoardSettings->getFieldCellSize());
}


void ExecuteUseCase::restartGame()
{
	const Point& archerPos = gameBoard->getArcherTower()->getPosition();
	const Point& towerPos = gameBoard->getTower()->getPosition();

	GameBoard* oldGameBoard = gameBoard;

	gameBoard = new GameBoard();
	gameBoard->createField(*oldGameBoard->getField());
	gameBoard->spawnArcherTower(oldGameBoard->getArcherTower()->getPosition());
	gameBoard->spawnTower(gameBoardSettings->getTowerHealth(), gameBoardSettings->getTowerRadius(), gameBoardSettings->getTowerSpawnTime());
	gameBoard->getTower()->setPosition(oldGameBoard->getTower()->getPosition());
	gameBoard->createPlayer(gameBoardSettings->getPlayerHealth(), gameBoardSettings->getPlayerDamage());

	delete oldGameBoard;
}


void ExecuteUseCase::checkSpawnNewEnemy()
{
	const auto* tower = gameBoard->getTower();
	if (gameBoard->getPlayer()->getSteps() % (tower->getSpawnTime()) == 0)
	{
		gameBoard->spawnEnemy(gameBoardSettings->getEnemyHealth(), gameBoardSettings->getEnemyDamage());
	}
}


void ExecuteUseCase::increasePlayerSteps()
{
	gameBoard->getPlayer()->increaseSteps();
}


void ExecuteUseCase::changePlayerLongFight()
{
	auto* player = gameBoard->getPlayer();
	player->setLongFight(!player->isLongFight());
}


void ExecuteUseCase::changePlayerFastMode()
{
	auto* player = gameBoard->getPlayer();
	player->setFastMode(!player->isFastMode());
}


const PointSystem& ExecuteUseCase::getPointSystem() const
{
	return pointSystem;
}


const GameBoard* ExecuteUseCase::getGameBoard() const
{
	return gameBoard;
}


bool ExecuteUseCase::checkGameEnd() const
{
	return checkGameWin() || checkGameOver();
}


bool ExecuteUseCase::checkGameOver() const
{
	return gameBoard->getPlayer()->getHealth() <= 0;
}


bool ExecuteUseCase::checkGameWin() const
{
	return gameBoard->getTower()->isDestroyed();
}


bool ExecuteUseCase::executePlayer(const Point& vector)
{
	auto* player = gameBoard->getPlayer();
	if (collisionDetector.checkMapCollision(*this, player->getPosition(), player->isFastMode() ? vector * player->getBoost() : vector))
		return false;

	if (collisionDetector.checkSlowerCellsCollision(*this) && !player->isSlowed())
	{
		player->setSlowed(true);
		return true;
	}

	if (player->isSlowed())
		player->setSlowed(false);
	

	if (!player->isSlowed() && !vector.isZero()) 
	{
		int points = pointSystem.getMoveCost(player->isFastMode());
		if (player->getScore() + points < 0)
			return false;

		player->addScore(points);
	}

	if(!vector.isZero())
		player->move(vector);

	return true;
}


bool ExecuteUseCase::executeArcherTower()
{
	auto* player = gameBoard->getPlayer();
	gameBoard->getArcherTower()->useSpell(*gameBoard, player->getPosition());
	return true;
}


bool ExecuteUseCase::executeAlly()
{
	auto& enemies = gameBoard->getEnemies();
	for (auto& ally : gameBoard->getAllies())
	{
		int index = 0;
		int maxDistance = 99999999;
		int distance = 0;
		Point allyPos = ally.getPosition();
		Point enemyPos;
		bool move = false;
		for (int i = 0; i < enemies.size(); ++i)
		{
			if (!enemies[i].isAlive())
				continue;

			enemyPos = enemies[i].getPosition();
			distance = std::abs(enemyPos.X() - allyPos.X()) + std::abs(enemyPos.Y() - allyPos.Y());

			if (distance <= 1)
			{
				ally.attack(enemies[i]);
				move = false;
				break;
			}

			if (distance < maxDistance)
			{
				index = i;
				maxDistance = distance;
				move = true;
			}
		}

		if (move)
		{
			Point vector;
			enemyPos = enemies[index].getPosition();
			/*find norm*/
			int dx = enemyPos.X() - allyPos.X();
			int dy = enemyPos.Y() - allyPos.Y();

			dx = dx != 0 ? dx / std::abs(dx) : 0;
			dy = dy != 0 ? dy / std::abs(dy) : 0;
			/*find norm*/

			srand((unsigned int)time(0));
			
			if (dx != 0 && dy != 0)
			{
				if (rand() % 2 == 0)
					vector.setX(dx);
				else
					vector.setY(dy);
			}

			if (dx == 0)
				vector.setY(dy);

			if (dy == 0)
				vector.setX(dx);

			while (collisionDetector.checkMapCollision(*this, allyPos, vector))
			{
				if (vector.X() != 0)
				{
					vector.setX(0);
					vector.setY(rand() % 2 ? -1 : 1);
				}
				else
				{
					vector.setY(0);
					vector.setX(rand() % 2 ? -1 : 1);
				}
			}

			ally.move(vector);
		}

	}
	return true;
}


bool ExecuteUseCase::executeEntity()
{
	executeAlly();

	srand((unsigned int)time(0));

	auto* player = gameBoard->getPlayer();

	auto* field = gameBoard->getField();
	
	Point playerPos = player->getPosition();

	auto& enemies = gameBoard->getEnemies();
	
	bool isSomeOneAttack = false;

	for (size_t i = 0; i < enemies.size();++i) {
		if (!enemies[i].isAlive())
			continue;

		const auto& enemyPos = enemies[i].getPosition();
		
		/*find norm*/
		int dx = playerPos.X() - enemyPos.X();
		int dy = playerPos.Y() - enemyPos.Y();

		dx = dx != 0 ? dx / std::abs(dx) : 0;
		dy = dy != 0 ? dy / std::abs(dy) : 0;
		/*find norm*/
		Point vector;

		if (dx != 0 && dy != 0) 
		{
			if (rand() % 2 == 0)
				vector.setX(dx);
			else 
				vector.setY(dy);
		}

		if (dx == 0)
			vector.setY(dy);

		if (dy == 0)
			vector.setX(dx);


		while(collisionDetector.checkMapCollision(*this, enemies[i].getPosition(), vector))
		{
			if (vector.X() != 0) 
			{
				vector.setX(0);
				vector.setY(rand() % 2 ? -1 : 1);
			}
			else
			{
				vector.setY(0);
				vector.setX(rand() % 2 ? -1 : 1);
			}
		}
		
		if (collisionDetector.checkEntitiesCollision(player->getPosition(), enemies[i].getPosition(), vector))
		{
			enemies[i].attack(*player);
			isSomeOneAttack = true;
			continue;
		}

		enemies[i].move(vector);
		
		if (collisionDetector.checkTrapsCollision(*this, enemies[i].getPosition()))
		{
			auto* trap = gameBoard->getTrap(enemies[i].getPosition());
			if (trap != nullptr)
			{
				enemies[i].takeDamage(trap->getDamage());
				trap->changeUsed();
			}
		}
	}

	executeArcherTower();

	if(isSomeOneAttack)
		return true;

	return false;
}


bool ExecuteUseCase::executePlayerAttack(Point& vector)
{
	if (vector.isZero())
		return true;

	auto* player = gameBoard->getPlayer();
	auto* field = gameBoard->getField();

	if (player->isLongFight())
		vector *= player->getLongFightDistance();

	Point need = player->getPosition() + vector;

	int points = pointSystem.getAttack(player->isLongFight());

	if (player->getScore() + points < 0)
		return false;


	bool isSomeOneAttack = false;
	//---------TOWER ATTACK--------
	auto* tower = gameBoard->getTower();

	if (tower->getPosition() == need) {
		player->attack(*tower);
		if (tower->isDestroyed())
		{
			field->changeObstruction(tower->getPosition());
		}
		isSomeOneAttack = true;
	}
	else 
	{
		//-----------ENEMIES ATTACK--------
		auto& enemies = gameBoard->getEnemies();
		for(size_t i = 0; i < enemies.size();++i)
		{
			if ((enemies[i].getPosition() == need) && enemies[i].isAlive()) {
				player->attack(enemies[i]);
				isSomeOneAttack = true;
				break;
			}
		}
	}
	player->addScore(points);
	return true;
}


void ExecuteUseCase::takeSpell(int index)
{
	auto* player = gameBoard->getPlayer();
	auto* hand = player->getHand();
	hand->takeSpell(index);
}


bool ExecuteUseCase::addSpell(SpellType type)
{
	auto* player = gameBoard->getPlayer();
	auto* hand = player->getHand();
	int points;
	switch (type)
	{
	case SpellType::DirectDamageSpell:
		points = pointSystem.getDirectSpellCost();
		if (player->getScore() + points >= 0)
		{
			player->addScore(points);
			hand->addSpell(type);
			return true;
		}
		break;
	case SpellType::AreaDamageSpell:
		points = pointSystem.getAreaSpellCost();
		if (player->getScore() + points >= 0)
		{
			player->addScore(points);
			hand->addSpell(type);
			return true;
		}
		break;
	case SpellType::TrapSpell:
		points = pointSystem.getTrapSpellCost();
		if (player->getScore() + points >= 0)
		{
			player->addScore(points);
			hand->addSpell(type);
			return true;
		}
		break;
	case SpellType::AllySpell:
		points = pointSystem.getAllySpellCost();
		if (player->getScore() + points >= 0)
		{
			player->addScore(points);
			hand->addSpell(type);
			return true;
		}
		break;
	case SpellType::UpgradeSpell:
		points = pointSystem.getUpgradeSpellCost();
		if (player->getScore() + points >= 0)
		{
			player->addScore(points);
			hand->addSpell(type);
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}


bool ExecuteUseCase::useSpell(const Point& destination)
{
	Point newDestination = destination / gameBoard->getField()->getCellSize();
	auto* hand = gameBoard->getPlayer()->getHand();
	if (newDestination == Point(0, 0))
		return hand->useSpell(*gameBoard, gameBoard->getPlayer()->getPosition());
	return hand->useSpell(*gameBoard, newDestination);
}


bool ExecuteUseCase::useSpell()
{
	auto* player = gameBoard->getPlayer();
	auto* hand = player->getHand();
	return hand->useSpell(*gameBoard, player->getPosition());
}


bool ExecuteUseCase::refresh()
{
	refreshTraps();
	refreshEntities();
	return true;
}


bool ExecuteUseCase::refreshTraps()
{
	std::vector<Trap> trapsUpdated;
	auto& traps = gameBoard->getTraps();

	for (size_t i = 0; i < traps.size(); ++i)
	{
		if (traps[i].isUsed())
			continue;
		trapsUpdated.push_back(traps[i]);
	}
	traps.clear();
	traps = trapsUpdated;
	return true;

}


bool ExecuteUseCase::refreshEntities()
{
	std::vector<Enemy> enemiesUpdated;
	std::vector<Ally> alliesUpdated;

	auto& allies = gameBoard->getAllies();
	auto& enemies = gameBoard->getEnemies();

	for (size_t i = 0; i < allies.size(); ++i)
	{
		if (!allies[i].isAlive())
			continue;

		alliesUpdated.push_back(allies[i]);
	}


	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (!enemies[i].isAlive() && (enemies[i].getDissapearTime() == 0))
			continue;

		if (!enemies[i].isAlive())
			enemies[i].reduceDissapearTime();
		enemiesUpdated.push_back(enemies[i]);
	}

	allies.clear();
	allies = alliesUpdated;

	enemies.clear();
	enemies = enemiesUpdated;
	return true;
}


ExecuteUseCase::~ExecuteUseCase()
{
	if (gameBoard != nullptr)
	{
		delete gameBoard;
		gameBoard = nullptr;
	}

	if (jsonManager != nullptr)
	{
		delete jsonManager;
		jsonManager = nullptr;
	}

	if (gameBoardSettings != nullptr)
	{
		delete gameBoardSettings;
		gameBoardSettings = nullptr;
	}
}