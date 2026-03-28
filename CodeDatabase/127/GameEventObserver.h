#ifndef GAMEEVENTOBSERVER_H
#define GAMEEVENTOBSERVER_H

#include "Logger.h"
#include <memory>
#include <vector>

// Наблюдатель за событиями игры (паттерн Observer)
class GameEventObserver {
private:
	std::vector<Logger*> loggers_;  

public:
	void addLogger(Logger* logger);
	void removeLogger(Logger* logger);
	
	// Методы для уведомления о событиях
	void onPlayerMove(int x, int y);
	void onPlayerAttack(int damage, int targetX, int targetY);
	void onEnemyDamaged(int enemyIndex, int damage);
	void onEnemyKilled(int enemyIndex);
	void onTowerDamaged(int towerIndex, int damage);
	void onSpellUsed(int spellIndex, int x, int y);
	void onSpellBought(const std::string& spellName);
	void onPlayerDamaged(int damage);
	void onLevelCleared(int level);
	void onGameSaved(const std::string& filename);
	void onGameLoaded(const std::string& filename);
};

#endif





