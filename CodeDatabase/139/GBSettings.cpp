#include "GBSettings.h"


GBSettings::GBSettings(unsigned int fieldWidth, unsigned int fieldHeight, unsigned int cellSize)
	: fieldBaseWidth(fieldWidth), fieldWidth(fieldWidth),
	fieldBaseHeight(fieldHeight), fieldHeight(fieldHeight),
	fieldBaseCellSize(cellSize), fieldCellSize(cellSize) { }


void GBSettings::setLevel(unsigned short level)
{
	this->level = level;
	switch (level)
	{
	case 1:
		fieldWidth = fieldBaseWidth;
		fieldHeight = fieldBaseHeight;

		playerHealth = playerBaseHealth;
		playerDamage = playerBaseDamage;
		break;
	case 2:
		fieldWidth = fieldBaseWidth + 3;
		fieldHeight = fieldBaseHeight + 1;
		break;
	case 3:
		fieldWidth = fieldBaseWidth + 5;
		fieldHeight = fieldBaseHeight + 3;
		break;
	case 4:
		fieldWidth = fieldBaseWidth + 5;
		fieldHeight = fieldBaseHeight + 5;
		break;
	case 5:
		fieldWidth = fieldBaseWidth + 7;
		fieldHeight = fieldBaseHeight + 5;
		break;
	case 6:
		fieldWidth = fieldBaseWidth + 10;
		fieldHeight = fieldBaseHeight + 5;
		break;
	case 7:
		fieldWidth = fieldBaseWidth + 10;
		fieldHeight = fieldBaseHeight + 10;
		break;
	default:
		fieldWidth = fieldBaseWidth;
		fieldHeight = fieldBaseHeight;
		break;
	}

	enemyHealth = (int)(10 * (level));
	enemyDamage = (int)(5 * (1. + 0.1 * level));;
	towerHealth = (int)50 * level;
	towerRadius = (int)(4 * (1 + 0.2 * level));
	towerSpawnTime = level >= 15 ? 5 : 20 - level;
}


void GBSettings::addPlayerHealth(unsigned int health)
{
	playerHealth += health;
}


void GBSettings::addPlayerDamage(unsigned int damage)
{
	playerDamage += damage;
}


unsigned short GBSettings::getLevel() const
{ 
	return level;
}


unsigned int GBSettings::getFieldWidth() const
{
	return fieldWidth;
}


unsigned int GBSettings::getFieldHeight() const
{
	return fieldHeight;
}


unsigned int GBSettings::getFieldCellSize() const
{
	return fieldCellSize;
}


unsigned int GBSettings::getPlayerHealth() const
{
	return playerHealth;
}


unsigned int GBSettings::getPlayerDamage() const
{
	return playerDamage;
}


unsigned int GBSettings::getEnemyHealth() const
{
	return enemyHealth;
}


unsigned int GBSettings::getEnemyDamage() const
{
	return enemyDamage;
}


unsigned int GBSettings::getTowerHealth() const
{
	return towerHealth;
}


unsigned int GBSettings::getTowerRadius() const
{
	return towerRadius;
}


unsigned int GBSettings::getTowerSpawnTime() const
{
	return towerSpawnTime;
}