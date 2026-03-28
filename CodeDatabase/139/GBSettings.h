#pragma once
#include "DataTypes/Point.h"


class GBSettings
{
private:
	unsigned short level = 1;
	unsigned int fieldWidth = 15;
	unsigned int fieldHeight = 15;
	unsigned int fieldCellSize = 20;
	unsigned int playerHealth = 100;
	unsigned int playerDamage = 20;
	unsigned int enemyHealth = 20;
	unsigned int enemyDamage = 20;
	unsigned int towerHealth = 50;
	unsigned int towerRadius = 20;
	unsigned int towerSpawnTime = 20;

	unsigned int fieldBaseWidth = 15;
	unsigned int fieldBaseHeight = 15;
	unsigned int fieldBaseCellSize = 20;
	unsigned int playerBaseHealth = 100;
	unsigned int playerBaseDamage = 20;
public:
	GBSettings(unsigned int fieldWidth, unsigned int fieldHeight, unsigned int cellSize);
	void setLevel(unsigned short level);
	void addPlayerHealth(unsigned int health);
	void addPlayerDamage(unsigned int damage);
	unsigned short getLevel() const;
	unsigned int getFieldWidth() const;
	unsigned int getFieldHeight() const;
	unsigned int getFieldCellSize() const;
	unsigned int getPlayerHealth() const;
	unsigned int getPlayerDamage() const;
	unsigned int getEnemyHealth() const;
	unsigned int getEnemyDamage() const;
	unsigned int getTowerHealth() const;
	unsigned int getTowerRadius() const;
	unsigned int getTowerSpawnTime() const;
};