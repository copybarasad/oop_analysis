#include "GameEventObserver.h"
#include <sstream>
#include <algorithm>

void GameEventObserver::addLogger(Logger* logger) {
	if (logger != nullptr) {
		loggers_.push_back(logger);
	}
}

void GameEventObserver::removeLogger(Logger* logger) {
	loggers_.erase(
		std::remove(loggers_.begin(), loggers_.end(), logger),
		loggers_.end()
	);
}

void GameEventObserver::onPlayerMove(int x, int y) {
	std::ostringstream oss;
	oss << "Игрок переместился в (" << x << ", " << y << ")";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onPlayerAttack(int damage, int targetX, int targetY) {
	std::ostringstream oss;
	oss << "Игрок атаковал цель на (" << targetX << ", " << targetY << "), нанесено " << damage << " урона";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onEnemyDamaged(int enemyIndex, int damage) {
	std::ostringstream oss;
	oss << "Враг #" << enemyIndex << " получил " << damage << " урона";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onEnemyKilled(int enemyIndex) {
	std::ostringstream oss;
	oss << "Враг #" << enemyIndex << " уничтожен";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onTowerDamaged(int towerIndex, int damage) {
	std::ostringstream oss;
	oss << "Башня #" << towerIndex << " получила " << damage << " урона";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onSpellUsed(int spellIndex, int x, int y) {
	std::ostringstream oss;
	oss << "Использовано заклинание #" << spellIndex << " на координатах (" << x << ", " << y << ")";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onSpellBought(const std::string& spellName) {
	std::ostringstream oss;
	oss << "Куплено заклинание: " << spellName;
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onPlayerDamaged(int damage) {
	std::ostringstream oss;
	oss << "Игрок получил " << damage << " урона";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onLevelCleared(int level) {
	std::ostringstream oss;
	oss << "Уровень " << level << " пройден";
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onGameSaved(const std::string& filename) {
	std::ostringstream oss;
	oss << "Игра сохранена в файл: " << filename;
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}

void GameEventObserver::onGameLoaded(const std::string& filename) {
	std::ostringstream oss;
	oss << "Игра загружена из файла: " << filename;
	for (Logger* logger : loggers_) {
		if (logger) logger->log(oss.str());
	}
}





