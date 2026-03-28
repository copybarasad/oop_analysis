#include "GameBoard.h"
#include <iostream>


GameBoard::GameBoard(nlohmann::json& js)
{
	if (!js.is_object())
		throw std::runtime_error("GameBoard::GameBoard: json is not an object");


	if (js.contains("Field"))
	{
		try {
			field = new Field(10, 10, 10);
			field->from_json(js["Field"]);
		}
		catch (std::exception& e) {
			throw std::runtime_error(u8"Не удалось загрузить Field в GameBoard. " + std::string(e.what()));
		}
	}
	else {
		throw std::invalid_argument(u8"Json некорректен: отсутствует Field");
	}


	if (js.contains("Tower"))
	{
		try {
			tower = new Tower();
			tower->from_json(js["Tower"]);
		}
		catch (std::exception& e) {
			throw std::runtime_error(u8"Не удалось загрузить Tower в GameBoard. " + std::string(e.what()));
		}
	}
	else {
		throw std::invalid_argument(u8"Json некорректен: отсутствует Tower");
	}


	if (js.contains("Player"))
	{
		try {
			player = new Player();
			player->from_json(js["Player"]);
		}
		catch (std::exception& e) {
			throw std::runtime_error(u8"Не удалось загрузить Player в GameBoard. " + std::string(e.what()));
		}
	}
	else {
		throw std::invalid_argument(u8"Json некорректен: отсутствует Player");
	}


	if (js.contains("ArcherTower"))
	{
		try {
			archerTower = new ArcherTower({0, 0});
			archerTower->from_json(js["ArcherTower"]);
		}
		catch (std::exception& e) {
			throw std::runtime_error(u8"Не удалось загрузить ArcherTower в GameBoard. " + std::string(e.what()));
		}
	}
	else {
		throw std::invalid_argument(u8"Json некорректен: отсутствует ArcherTower");
	}


	if (js.contains("Enemies"))
	{
		try {
			for (auto& jEnemy : js["Enemies"]) {
				Enemy enemy;
				enemy.from_json(jEnemy);
				enemies.push_back(enemy);
			}
		}
		catch (std::exception& e) {
			throw std::runtime_error(u8"Не удалось загрузить Enemies в GameBoard. " + std::string(e.what()));
		}
	}
	else {
		throw std::invalid_argument(u8"Json некорректен: отсутствует Enemies");
	}


	if (js.contains("Allies"))
	{
		try {
			for (auto& jAlly : js["Allies"]) {
				Ally ally;
				ally.from_json(jAlly);
				allies.push_back(ally);
			}
		}
		catch (std::exception& e) {
			throw std::runtime_error(u8"Не удалось загрузить Allies в GameBoard. " + std::string(e.what()));
		}
	}
	else {
		throw std::invalid_argument(u8"Json некорректен: отсутствует Allies");
	}
	

	if (js.contains("Traps"))
	{
		try {
			for (auto& jTrap : js["Traps"]) {
				Trap trap({ 0, 0 });
				trap.from_json(jTrap);
				traps.push_back(trap);
			}
		}
		catch (std::exception& e) {
			throw std::runtime_error(u8"Не удалось загрузить Traps в GameBoard. " + std::string(e.what()));
		}
	}
	else {
		throw std::invalid_argument(u8"Json некорректен: отсутствует Traps");
	}
}


void GameBoard::createField(int width, int height, int cellSize)
{
	field = new Field(width, height, cellSize);
	initializeField();
	Point center = Point(field->getWidth() / 2, field->getHeight() / 2);
	for (auto& point : field->generatePositions(center, 1, 4))
	{
		spawnArcherTower(point);
		field->changeObstruction(point);
	}
}


void GameBoard::createField(const Field& field)
{
	this->field = new Field(field);
}


void GameBoard::createPlayer(unsigned int health, unsigned int damage)
{
	player = new Player(health, damage);
	spawnPlayer();
}


void GameBoard::createPlayer(unsigned int health, unsigned int damage, Point& pos)
{
	player = new Player(health, damage);
	if (field->getCell(pos).isEmpty())
		player->setPosition(pos);
	else
		spawnPlayer();
}


void GameBoard::createPlayer(const Player& player)
{
	if (this->player)
	{
		delete this->player;
		this->player = nullptr;
	}

	this->player = new Player(player);
	spawnPlayer();
}


void GameBoard::spawnPlayer()
{
	for (const auto& position : field->generatePositions(1, true))
		player->setPosition(position);
}


void GameBoard::spawnArcherTower(const Point& pos)
{
	archerTower = new ArcherTower(pos);
}


void GameBoard::initializeField()
{
	field->generateObstructionCells(-1);
	field->generateSlowCells(40);
}


void GameBoard::spawnTower(unsigned int health, unsigned int radius, unsigned int spawnTime)
{
	//fix
	Point p(field->getWidth() - 1 - (rand() % 2 + 1), (rand() % (field->getHeight() - 2)) + 1);
	tower = new Tower(health, radius, spawnTime);
	tower->setPosition(p);
	if (field->getCell(p).isObstruction())
		field->changeObstruction(p);
}


void GameBoard::spawnEnemy(unsigned int health, unsigned int damage)
{
	for (auto& pos : field->generatePositions(tower->getPosition(), 1, tower->getRadius()))
	{
		Enemy enemy(health, damage);
		enemy.setPosition(pos);
		enemies.push_back(enemy);
	}
}


void GameBoard::spawnAlly(int health, int damage, int alliesCount, const Point& point)
{	
	for (auto& pos : field->generatePositions(point, alliesCount, 2))
		allies.push_back(Ally(health, damage, pos));
}


std::vector<Enemy*> GameBoard::getEnemiesAt(const Point& point, int radius)
{
	int left = point.X() - radius;
	int right = point.X() + radius;
	int down = point.Y() + radius;
	int up = point.Y() - radius;

	std::vector<Enemy*> selectedEnemies;
	for (auto& enemy : enemies)
	{
		auto pos = enemy.getPosition();
		if (left <= pos.X() && pos.X() <= right && up <= pos.Y() && pos.Y() <= down)
			selectedEnemies.push_back(&enemy);
	}

	return selectedEnemies;
}


const std::vector<Ally>& GameBoard::getAllies() const
{
	return allies;
}


std::vector<Ally>& GameBoard::getAllies()
{
	return allies;
}


const std::vector<Enemy>& GameBoard::getEnemies() const
{
	return enemies;
}


std::vector<Enemy>& GameBoard::getEnemies()
{
	return enemies;
}


const std::vector<Trap>& GameBoard::getTraps() const
{
	return traps;
}


std::vector<Trap>& GameBoard::getTraps()
{
	return traps;
}


Trap* GameBoard::getTrap(const Point& point)
{
	for (auto& trap : traps)
	{
		if (trap.getPosition() == point)
			return &trap;
	}
	return nullptr;
}


Entity* GameBoard::getEntityAt(const Point& point)
{
	if (player->getPosition() == point)
		return player;

	return nullptr;
}


Enemy* GameBoard::getEnemyAt(const Point& point)
{
	for (auto& enemy : enemies)
	{
		if (enemy.getPosition() == point)
			return &enemy;
	}
	return nullptr;
}


Tower* GameBoard::getTowerAt(const Point& point, int radius)
{
	auto& position = tower->getPosition();

	int left = point.X() - radius;
	int right = point.X() + radius;
	int down = point.Y() + radius;
	int up = point.Y() - radius;

	if (left <= position.X() && position.X() <= right && up <= position.Y() && position.Y() <= down)
		return tower;
	return nullptr;
}


Tower* GameBoard::getTowerAt(const Point& point)
{
	if (point == tower->getPosition())
		return tower;
	return nullptr;
}


bool GameBoard::spawnTrap(const Point& point)
{
	int x = point.X();
	int y = point.Y();
	if (!(0 <= x && x < field->getWidth() && 0 <= y && y < field->getHeight()))
		return false;

	if (field->getCell(point).isEmpty())
	{
		for (auto& trap : traps)
		{
			if (trap.getPosition() == point)
				return false;
		}
		traps.push_back(Trap(point));
		return true;
	}

	return false;
}


void  GameBoard::upgradeSpell(int count)
{
	player->getHand()->upgradeSpell(count);
}


const Enemy* GameBoard::getEnemy(const Point& pos) const
{
	for (auto& enemy : enemies)
	{
		if (enemy.getPosition() == pos)
			return &enemy;
	}
	return nullptr;
}


const Entity* GameBoard::getEntityById(const int id) const
{
	if (player->getId() == id)
		return player;

	for (auto& enemy : enemies)
	{
		if (enemy.getId() == id)
			return &enemy;
	}
	return nullptr;
}


Field* GameBoard::getField()
{
	return field;
}


const Field* GameBoard::getField() const
{
	return field;
}


Player* GameBoard::getPlayer()
{
	return player;
}


const Player* GameBoard::getPlayer() const
{
	return player;
}


const ArcherTower* GameBoard::getArcherTower() const
{
	return archerTower;
}


ArcherTower* GameBoard::getArcherTower()
{
	return archerTower;
}




Tower* GameBoard::getTower()
{
	return tower;
}


const Tower* GameBoard::getTower() const
{
	return tower;
}


GameBoard::~GameBoard()
{
	if (player)
	{
		delete player;
		player = nullptr;
	}

	if (field)
	{
		delete field;
		field = nullptr;
	}

	if (archerTower)
	{
		delete archerTower;
		archerTower = nullptr;
	}

	if (tower)
	{
		delete tower;
		tower = nullptr;
	}
}
