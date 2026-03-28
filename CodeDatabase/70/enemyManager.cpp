#include "enemyManager.h"

void EnemyManager::addEnemy(int health, int damage, int reward, int x, int y)
{
	Enemy enemy(health, damage, reward);
	std::pair<int, int> enemyPos = {x, y};
	enemies[enemyPos] = enemy;
}

void EnemyManager::removeEnemy(int x, int y)
{
	std::pair<int, int> enemyPos = {x, y};
	enemies.erase(enemyPos);
}

bool EnemyManager::isEnemyInPos(int x, int y) const
{
	std::pair<int, int> enemyPos = {x, y};
	auto it = enemies.find(enemyPos);
	return !(it == enemies.end());
}

bool EnemyManager::empty() const
{
	return enemies.empty();
}

int EnemyManager::getReward(int x, int y)
{
	std::pair<int, int> enemyPos = {x, y};
	auto it = enemies.find(enemyPos);
	return (it->second).getPoints();
}

int EnemyManager::getEnemyHealth(int x, int y)
{
	std::pair<int, int> enemyPos = {x, y};
	auto it = enemies.find(enemyPos);
	return (it->second).getHealth();
}

void EnemyManager::receiveDamage(int x, int y, int damage)
{
	std::pair<int, int> enemyPos = {x, y};
	auto it = enemies.find(enemyPos);
	(it->second).receiveDamage(damage);
}

bool EnemyManager::isEnemyAlive(int x, int y)
{
	std::pair<int, int> enemyPos = {x, y};
	auto it = enemies.find(enemyPos);
	return (it->second).getHealth() > 0;
}

void EnemyManager::setEnemyPos(int old_x, int old_y, int new_x, int new_y)
{
	std::pair<int, int> enemyPos = {old_x, old_y};
	auto it = enemies.find(enemyPos);
	Enemy enemy = it->second;
	std::pair<int, int> newEnemyPos = {new_x, new_y};
	enemies.erase(enemyPos);
	enemies[newEnemyPos] = enemy;
}

std::map<std::pair<int, int>, Enemy> EnemyManager::getEnemies()
{
	return enemies;
}

const std::map<std::pair<int, int>, Enemy> EnemyManager::getEnemies() const
{
	return enemies;
}

int EnemyManager::getEnemyDamage(int x, int y)
{
	std::pair<int, int> enemyPos = {x, y};
	auto it = enemies.find(enemyPos);
	return (it->second).getDamage();
}
void EnemyManager::showEnemies() const
{
	for (const auto& [enemyPos, enemy] : enemies)
	{
		std::cout << std::format("Cell: {} {}: ", enemyPos.first, enemyPos.second);
		enemy.showStats();
	}
}

int EnemyManager::attackEnemy(int x, int y, int damage)
{
	int reward = 0;
	receiveDamage(x, y, damage);
	std::cout << std::format("Enemy at pos ({};{}) attacked! -{} health points!", x, y, damage) << std::endl;
	if (!isEnemyAlive(x, y))
	{
		reward = getReward(x, y);
		removeEnemy(x, y);
	}
	return reward;
}

void EnemyManager::setEnemies(std::map<std::pair<int, int>, Enemy> enemies)
{
	this->enemies = enemies;
}

size_t EnemyManager::getEnemiesNum() const
{
	return enemies.size();
}

std::ostream& operator << (std::ostream &os, const std::pair<int, int> &pair);
std::istream& operator >> (std::istream& in, std::pair<int, int>& pair);

std::istream& operator >> (std::istream &in, Enemy& enemy);
std::ostream& operator << (std::ostream &os, const Enemy &enemy);

std::ostream& operator << (std::ostream &os, const EnemyManager &enemyManager)
{
	os << enemyManager.getEnemiesNum() << " ";
	std::map<std::pair<int, int>, Enemy> enemies = enemyManager.getEnemies();
	for (const auto& [enemyPos, enemy] : enemies)
	{
		os << enemyPos << " " << enemy;
	}
	return os;
}
std::istream& operator >> (std::istream& in, EnemyManager& enemyManager)
{
	std::map<std::pair<int, int>, Enemy> enemies;
	size_t enemiesNum;
	in >> enemiesNum;
	if (!in.good()) throw std::runtime_error("Failed to read num of enemies");
	if(enemiesNum < 0)
	{
		throw std::invalid_argument("Wrong arguments for EnemyManager!");
	}
	std::pair<int, int> enemyPos;
	Enemy enemy;
	for(size_t i = 0; i < enemiesNum; ++i)
	{
		in >> enemyPos >> enemy;
		if (!in.good()) throw std::runtime_error("Failed to read key-value in enemy manager");
		enemies[enemyPos] = enemy;
	}
	enemyManager.setEnemies(enemies);
	return in;
}