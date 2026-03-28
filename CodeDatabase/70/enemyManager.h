#include "enemy.h"
#include <utility>
#include <map>
#include <iostream>

#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

class EnemyManager
{
	private:
		std::map<std::pair<int, int>, Enemy> enemies;
	public:
		void addEnemy(int health, int damage, int reward, int x, int y);
		void removeEnemy(int x, int y);
		
		int getReward(int x, int y);
		int getEnemyHealth(int x, int y);
		int getEnemyDamage(int x, int y);
		std::map<std::pair<int, int>, Enemy> getEnemies();
		const std::map<std::pair<int, int>, Enemy> getEnemies() const;
		size_t getEnemiesNum() const;
		void setEnemies(std::map<std::pair<int, int>, Enemy> enemies);

		void receiveDamage(int x, int y, int damage);

		bool empty() const;
		bool isEnemyAlive(int x, int y);
		bool isEnemyInPos(int x, int y) const;
		
		void setEnemyPos(int old_x, int old_y, int new_x, int new_y);
		
		void showEnemies() const;
		int attackEnemy(int x, int y, int damage);
};

#endif